#include "model_wrapper.h"
#include "model_data_qs.h"

unsigned char* get_resources_model_onnx() {
	return model_con2_8_80_2_quant_cp_with_runtime_opt_ort;
}

unsigned int get_resources_model_onnx_len() {
	return model_con2_8_80_2_quant_cp_with_runtime_opt_ort_len;
}