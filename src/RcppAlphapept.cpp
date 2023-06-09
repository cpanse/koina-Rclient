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


namespace {

void
ValidateShapeAndDatatype(
    const std::string& name, std::shared_ptr<tc::InferResult> result)
{
  std::vector<int64_t> shape;
  FAIL_IF_ERR(
      result->Shape(name, &shape), "unable to get shape for '" + name + "'");
  // Validate shape
  if ((shape.size() != 3) || (shape[0] < 1) || (shape[1] < 1)) {
    Rcpp::Rcerr << "error: received incorrect shapes for '" << name << "'"
              << std::endl;
   exit(1);
  }
  std::string datatype;
  FAIL_IF_ERR(
      result->Datatype(name, &datatype),
      "unable to get datatype for '" + name + "'");
  // Validate datatype
  if (datatype.compare("FP32") != 0) {
    std::cerr << "error: received incorrect datatype for '" << name
              << "': " << datatype << std::endl;
    exit(1);
  }
}

}  // namespace

//' AlphaPept_ms2_generic_ensemble
//'
//' This function returns a predicted fragment ion spectrum
//' of a given peptide sequence.
//'
//' @param peptide amino acid sequence, e.g., \code{"LGGNEQVTR"}
//' @param precursorCharge precursor charge, e.g., 2.
//' @param collisionEnergy the collision energy in [eV]?.
//' @param instrument  an integer defining the instrument: 0 - QE;
//' 1 - Lumos; 2 - timsTOF; 3 - SciexTOF.
//' @param verbose provides grpc proteobuf output.
//' @param url GRPC url, e.g., localhost:9990 or \url{dlomix.fgcz.uzh.ch:8080}.
//' 
//' @examples
//'   dlomix::alphaPeptMs2GenericEnsemble("LGGNEQVTR")       
//' 
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector alphaPeptMs2GenericEnsemble(
  Rcpp::StringVector peptide,
  float collisionEnergy = 0.25,
  int precursorCharge = 2,
  int instrument = 0,
  bool verbose = false,
  std::string url = "dlomix.fgcz.uzh.ch:8080")
{
  long int batch_size = 1;
  tc::Headers http_headers;
  uint32_t client_timeout = 0;
  bool use_ssl = false;
  tc::SslOptions ssl_options;
  tc::KeepAliveOptions keepalive_options;

  // We use a simple model that takes 4 input tensors of x charaters
  // each and returns 1 output tensors of shape1 * shape2 * shape3 FP32.
  std::string model_name = "AlphaPept_ms2_generic_ensemble";
  //std::string model_name = "Prosit_2019_intensity_ensemble";
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
  std::vector<int32_t> input2_data(batch_size);
  std::vector<int64_t> input3_data(batch_size);

  for (auto i = 0; i < batch_size; ++i) {
    input0_data[i] = peptide[0];
    input1_data[i] = collisionEnergy;
    input2_data[i] = precursorCharge;

//     https://github.com/MannLabs/alphapeptdeep/blob/f30d57f07e1e14213144e2e7b407d90c11254882/peptdeep/constants/model_const.yaml#L116 <https://github.com/MannLabs/alphapeptdeep/blob/f30d57f07e1e14213144e2e7b407d90c11254882/peptdeep/constants/model_const.yaml#L116>

    input3_data[i] = instrument;
  }

  std::vector<int64_t> shape{batch_size, 1};

  // Initialize the inputs with the data.
  tc::InferInput* input0;
  tc::InferInput* input1;
  tc::InferInput* input2;
  tc::InferInput* input3;

// create
  FAIL_IF_ERR(
      tc::InferInput::Create(&input0, "peptides_in_str:0", shape, "BYTES"),
      "unable to get INPUT0");
  std::shared_ptr<tc::InferInput> input0_ptr;
  input0_ptr.reset(input0);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input1, "collision_energy_in:0", shape, "INT32"),
      "unable to get INPUT1");
  std::shared_ptr<tc::InferInput> input1_ptr;
  input1_ptr.reset(input1);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input2, "precursor_charge_in_int:0", shape, "INT32"),
      "unable to get INPUT2");
  std::shared_ptr<tc::InferInput> input2_ptr;
  input2_ptr.reset(input2);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input3, "instrument_indices:0", shape, "INT64"),
      "unable to get INPUT3");
  std::shared_ptr<tc::InferInput> input3_ptr;
  input3_ptr.reset(input3);


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
          input2_data.size() * sizeof(int32_t)),
      "unable to set data for INPUT2");


  FAIL_IF_ERR(
      input3_ptr->AppendRaw(
          reinterpret_cast<uint8_t*>(&input3_data[0]),
          input3_data.size() * sizeof(int64_t)),
      "unable to set data for INPUT3");


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

  std::vector<tc::InferInput*> inputs = {input0_ptr.get(), input1_ptr.get(), input2_ptr.get(), input3_ptr.get()};
  std::vector<const tc::InferRequestedOutput*> outputs = {output0_ptr.get(), output1_ptr.get()};

  tc::InferResult* results;
  FAIL_IF_ERR(
      client->Infer(&results, options, inputs, outputs, http_headers),
      "unable to run model");
  std::shared_ptr<tc::InferResult> results_ptr;
  results_ptr.reset(results);

 // std::cout << results_ptr->DebugString() << std::endl;
  // Validate the results...
 // ValidateShapeAndDatatype("out/Reshape:0", results_ptr);

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
