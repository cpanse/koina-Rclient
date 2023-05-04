// C++ grpc client demo 2023-04-30
// Christian Panse <cp@fgcz.ethz.ch>
// https://github.com/cpanse/dlomix-Rclient
// adapted from https://github.com/triton-inference-server/client/blob/main/src/c%2B%2B/examples/simple_grpc_string_infer_client.cc // Copyright (c) 2020-2021, NVIDIA CORPORATION. All rights reserved.

#include <getopt.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "grpc_client.h"
#include <Rcpp.h>

namespace tc = triton::client;

#define FAIL_IF_ERR(X, MSG)                                          \
  {                                                                  \
    tc::Error err = (X);                                             \
    if (!err.IsOk()) {                                               \
      Rcpp::Rcerr << "error: " << (MSG) << ": " << err << std::endl; \
      Rcpp::stop("stop.");                                           \
    }                                                                \
  }


//' Prosit_2019_intensity_ensemble
//'
//' This function returns a predicted fragment ion spectrum
//' of a given peptide sequence.
//'
//' @inheritParams alphaPeptMs2GenericEnsemble
//'
//' @examples
//'   dlomix::prosit2019IntensityEnsemble("LGGNEQVTR")       
//' 
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector prosit2019IntensityEnsemble(
  Rcpp::StringVector peptide,
  float collisionEnergy = 25,
  int precursorCharge = 2,
  bool verbose = false,
  std::string url = "dlomix.fgcz.uzh.ch:8080")
{
  long int batch_size = 1;
  tc::Headers http_headers;
  uint32_t client_timeout = 0;
  bool use_ssl = false;
  tc::SslOptions ssl_options;
  tc::KeepAliveOptions keepalive_options;

  std::string model_name = "Prosit_2019_intensity_ensemble";
  std::string model_version = "";

  // Create a InferenceServerGrpcClient instance to communicate with the
  // server using gRPC protocol.
  std::unique_ptr<tc::InferenceServerGrpcClient> client;
  FAIL_IF_ERR(
      tc::InferenceServerGrpcClient::Create(
          &client, url, verbose, use_ssl, ssl_options, keepalive_options),
      "unable to create grpc client");

  // Create the data for the two input tensors. Initialize the first
  // to unique integers and the second to all ones.
  std::vector<std::string> input0_data(batch_size);
  std::vector<int32_t> input1_data(batch_size);
  std::vector<float> input2_data(batch_size);

  for (auto i = 0; i < batch_size; ++i) {
    input0_data[i] = peptide[0];
    input1_data[i] = precursorCharge;
    input2_data[i] = collisionEnergy;
  }

  std::vector<int64_t> shape{batch_size, 1};

  // Initialize the inputs with the data.
  tc::InferInput* input0;
  tc::InferInput* input1;
  tc::InferInput* input2;

// create
  FAIL_IF_ERR(
      tc::InferInput::Create(&input0, "peptides_in_str:0", shape, "BYTES"),
      "unable to get INPUT0");
  std::shared_ptr<tc::InferInput> input0_ptr;
  input0_ptr.reset(input0);


  FAIL_IF_ERR(
      tc::InferInput::Create(&input1, "precursor_charge_in_int:0", shape, "INT32"),
      "unable to get precursor_charge_in_int:0");
  std::shared_ptr<tc::InferInput> input1_ptr;
  input1_ptr.reset(input1);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input2, "collision_energy_in:0", shape, "FP32"),
      "unable to get collision_energy_in:");
  std::shared_ptr<tc::InferInput> input2_ptr;
  input2_ptr.reset(input2);


/// assign
  FAIL_IF_ERR(
      input0_ptr->AppendFromString(input0_data),
      "unable to set data for INPUT0");

  FAIL_IF_ERR(
      input1_ptr->AppendRaw(
          reinterpret_cast<uint8_t*>(&input1_data[0]),
          input1_data.size() * sizeof(int32_t)),
      "unable to set data for INPUT1");

  FAIL_IF_ERR(
      input2_ptr->AppendRaw(
          reinterpret_cast<uint8_t*>(&input2_data[0]),
          input2_data.size() * sizeof(float)),
      "unable to set data for INPUT2");


  // Generate the outputs to be requested.
  tc::InferRequestedOutput* output0;
  tc::InferRequestedOutput* output1;

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output0, "out/Reshape:1"),
      "unable to get 'OUTPUT1'");
  std::shared_ptr<tc::InferRequestedOutput> output0_ptr;
  output0_ptr.reset(output0);

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output1, "out/Reshape:2"),
      "unable to get 'OUTPUT2'");
  std::shared_ptr<tc::InferRequestedOutput> output1_ptr;
  output1_ptr.reset(output1);


  // The inference settings. Will be using default for now.
  tc::InferOptions options(model_name);
  options.model_version_ = model_version;
  options.client_timeout_ = client_timeout;

  std::vector<tc::InferInput*> inputs = {input0_ptr.get(), input1_ptr.get(), input2_ptr.get()};
  std::vector<const tc::InferRequestedOutput*> outputs = {output0_ptr.get(), output1_ptr.get()};

  tc::InferResult* results;
  FAIL_IF_ERR(
      client->Infer(&results, options, inputs, outputs, http_headers),
      "unable to run model");
  std::shared_ptr<tc::InferResult> results_ptr;
  results_ptr.reset(results);

  // Get output
  float* output0_data;
  size_t output0_byte_size;
  FAIL_IF_ERR(
    results_ptr->RawData("out/Reshape:1",
      (const uint8_t**)&output0_data, &output0_byte_size),
      "unable to get result data for 'OUTPUT1'");  

  float* output1_data;
  size_t output1_byte_size;
  FAIL_IF_ERR(
    results_ptr->RawData("out/Reshape:2",
      (const uint8_t**)&output1_data, &output1_byte_size),
      "unable to get result data for 'OUTPUT2'");  

  //Rcpp::Rcout << "output0_byte_size\t=\t" << output0_byte_size<< std::endl;
 
  Rcpp::NumericVector out(output0_byte_size / 2);
  for (size_t i = 0; i < output1_byte_size / 4; ++i) {
	  out[i] = output0_data[i];
	  out[(output1_byte_size / 4) + i + 1] = output1_data[i];
  }

  return out;
}

//' Prosit_2019_irt_ensemble
//'
//' @inheritParams alphaPeptMs2GenericEnsemble
//'
//' @examples
//'   dlomix::prosit2019IrtEnsemble("LGGNEQVTR")       
//' 
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector prosit2019IrtEnsemble(
  Rcpp::StringVector peptide,
  bool verbose = false,
  std::string url = "dlomix.fgcz.uzh.ch:8080")
{
  long int batch_size = 1;
  tc::Headers http_headers;
  uint32_t client_timeout = 0;
  bool use_ssl = false;
  tc::SslOptions ssl_options;
  tc::KeepAliveOptions keepalive_options;

  std::string model_name = "Prosit_2019_irt_ensemble";
  std::string model_version = "";

  // Create a InferenceServerGrpcClient instance to communicate with the
  // server using gRPC protocol.
  std::unique_ptr<tc::InferenceServerGrpcClient> client;
  FAIL_IF_ERR(
      tc::InferenceServerGrpcClient::Create(
          &client, url, verbose, use_ssl, ssl_options, keepalive_options),
      "unable to create grpc client");

  // Create the data for the two input tensors. Initialize the first
  // to unique integers and the second to all ones.
  std::vector<std::string> input0_data(batch_size);

  for (auto i = 0; i < batch_size; ++i) {
    input0_data[i] = peptide[0];
  }

  std::vector<int64_t> shape{batch_size, 1};

  // Initialize the inputs with the data.
  tc::InferInput* input0;

// create
  FAIL_IF_ERR(
      tc::InferInput::Create(&input0, "in/proforma", shape, "BYTES"),
      "unable to get INPUT0");
  std::shared_ptr<tc::InferInput> input0_ptr;
  input0_ptr.reset(input0);


/// assign
  FAIL_IF_ERR(
      input0_ptr->AppendFromString(input0_data),
      "unable to set data for INPUT0");

  // Generate the outputs to be requested.
  tc::InferRequestedOutput* output0;

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output0, "out/irt"),
      "unable to get 'out/irt'");
  std::shared_ptr<tc::InferRequestedOutput> output0_ptr;
  output0_ptr.reset(output0);


  // The inference settings. Will be using default for now.
  tc::InferOptions options(model_name);
  options.model_version_ = model_version;
  options.client_timeout_ = client_timeout;

  std::vector<tc::InferInput*> inputs = {input0_ptr.get()};
  std::vector<const tc::InferRequestedOutput*> outputs = {output0_ptr.get()};

  tc::InferResult* results;
  FAIL_IF_ERR(
      client->Infer(&results, options, inputs, outputs, http_headers),
      "unable to run model");
  std::shared_ptr<tc::InferResult> results_ptr;
  results_ptr.reset(results);

  // Get output
  float* output0_data;
  size_t output0_byte_size;
  FAIL_IF_ERR(
    results_ptr->RawData("out/irt",
      (const uint8_t**)&output0_data, &output0_byte_size),
      "unable to get result data for 'OUTPUT1'");  

  //Rcpp::Rcout << "output0_byte_size\t=\t" << output0_byte_size<< std::endl;
 
  Rcpp::NumericVector out(1);
  //for (size_t i = 0; i < output0_byte_size / 4; ++i) {
	  out[0] = output0_data[0];
  //}

  return out;
}
