#include "tglang.hpp"

#include <dlfcn.h>
#include <onnxruntime_cxx_api.h>
#include <cpu_provider_factory.h>
// #include <chrono>

#include "GPT2Tokenizer.hpp"
#include "model_wrapper.h"

#define BATCH_SIZE 1
#define INPUT_DATA_SIZE 256
#define PAD_TOKEN_INDEX 50257
#define NUM_CORES 9

template <typename T>
T unwrap(std::optional<T>&& value, const std::string& error_msg) {
	if (value.has_value()) {
		return value.value();
	}
	else {
		throw std::runtime_error(error_msg);
	}
} 

static Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
static std::unique_ptr<GPT2Tokenizer> tokenizer = nullptr;
static std::unique_ptr<Ort::Session> onnx_session = nullptr;

void my_init_function(void) {
	std::chrono::steady_clock::time_point start_time, end_time;
	std::chrono::duration<double, std::milli> elapsed_time;
	start_time = std::chrono::steady_clock::now();


	GPT2Tokenizer loadedTokenizer = unwrap(GPT2Tokenizer::load(), "Error initializing GPT2 tokenizer\n");

	// Wrap the loaded tokenizer in a std::unique_ptr
	tokenizer = std::make_unique<GPT2Tokenizer>(loadedTokenizer);


	end_time = std::chrono::steady_clock::now();
	elapsed_time = end_time - start_time;
	printf("Time to load tokenizer: %.4f ms\n", elapsed_time.count());
	start_time = std::chrono::steady_clock::now();


	OrtEnv* environment;
	OrtThreadingOptions* envOpts = nullptr;
	Ort::GetApi().CreateThreadingOptions(&envOpts);
	Ort::GetApi().SetGlobalIntraOpNumThreads(envOpts, NUM_CORES);
	Ort::GetApi().SetGlobalSpinControl(envOpts, 1);
	Ort::GetApi().CreateEnvWithGlobalThreadPools(ORT_LOGGING_LEVEL_WARNING, "test", envOpts, &environment);
	Ort::Env env(ORT_LOGGING_LEVEL_ERROR, "test");
	Ort::SessionOptions session_options;
	session_options.SetIntraOpNumThreads(NUM_CORES);
	session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL); //ORT_ENABLE_ALL); // For speed up 
	session_options.SetExecutionMode(ExecutionMode::ORT_PARALLEL);
	uint32_t filter_flags = 0;  // 0 means use all available providers
	Ort::GetAvailableProviders();

	
	end_time = std::chrono::steady_clock::now();
	elapsed_time = end_time - start_time;
	printf("Initialization time: %.4f ms\n", elapsed_time.count());
	start_time = std::chrono::steady_clock::now();
	
	onnx_session = std::make_unique<Ort::Session>(env, get_resources_model_onnx(), get_resources_model_onnx_len(), session_options);

	end_time = std::chrono::steady_clock::now();
	elapsed_time = end_time - start_time;
	printf("Model prepare time: %.4f ms\n", elapsed_time.count());
}

// static bool inited = [](){
//  my_init_function();
//  return true;
// }();

void preprocess_text(const char* text, int64_t** input_data, size_t* input_data_size, int64_t* input_dims, size_t* num_input_dims) {
	auto token_ids = tokenizer->encode(text, 256);
	
	// Ensure the token_ids size is 64, padding with PAD_TOKEN_INDEX if necessary
	token_ids.resize(INPUT_DATA_SIZE, PAD_TOKEN_INDEX);
	
	// Set the fixed input_data_size
	*input_data_size = INPUT_DATA_SIZE;
	
	*input_data = new int64_t[*input_data_size];
	if (token_ids.size() < *input_data_size) {
		printf("Error: Source data size is less than destination array size.\n");
		// Handle error
	}
	memcpy(*input_data, token_ids.data(), *input_data_size * sizeof(long));

	input_dims[0] = BATCH_SIZE;
	input_dims[1] = BATCH_SIZE * *input_data_size;
	*num_input_dims = 2;

	// for (size_t i = 0; i < *input_data_size; ++i) {
	// 	printf("%ld ", (*input_data)[i]);
	// }
	// printf("\n");

	// std::string decoded_text = tokenizer->decode(token_ids);
	// printf("Decoded Text: %s\n", decoded_text.c_str());
}

enum TglangLanguage tglang_detect_programming_language(const char *text) {
	my_init_function();
	
	std::chrono::steady_clock::time_point start_time, end_time;
	std::chrono::duration<double, std::milli> elapsed_time;
	start_time = std::chrono::steady_clock::now();

	int64_t* input_data;
	size_t input_data_size;
	int64_t input_dims[2];
	size_t num_input_dims = 2;

	preprocess_text(text, &input_data, &input_data_size, input_dims, &num_input_dims);
	auto input_tensor = Ort::Value::CreateTensor<int64_t>(memory_info, input_data, input_data_size, input_dims, num_input_dims);

	std::vector<const char*> input_node_names = {"input"};
	std::vector<const char*> output_node_names = {"output"};
	Ort::Value output_tensor{nullptr};

	end_time = std::chrono::steady_clock::now();
	elapsed_time = end_time - start_time;
	printf("Preprocess text time: %.4f ms\n", elapsed_time.count());
	start_time = std::chrono::steady_clock::now();

	onnx_session->Run(Ort::RunOptions{nullptr}, input_node_names.data(), &input_tensor, 1, output_node_names.data(), &output_tensor, 1);
	
	float* floatarr = output_tensor.GetTensorMutableData<float>();
	int8_t max_index = 0;
	float max_value = floatarr[0];
	for (int8_t i = 1; i < TGLANG_LANGUAGE_COUNT; ++i) {
		printf("category %d: %.5f\n", i, floatarr[i]);
		if (floatarr[i] > max_value) {
			max_value = floatarr[i];
			max_index = i;
		}
	}

	end_time = std::chrono::steady_clock::now();
	elapsed_time = end_time - start_time;
	printf("Run time: %.4f ms\n", elapsed_time.count());

	if(max_value < 0.6) {
		return (enum TglangLanguage)TGLANG_LANGUAGE_OTHER;
	}
	return (enum TglangLanguage)max_index;
}
