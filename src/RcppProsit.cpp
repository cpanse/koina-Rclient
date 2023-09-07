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


Rcpp::DataFrame extractPrositPredictedSpectrum(float* output0_data, float* output1_data,
                                               const std::vector<std::string>&output2_data,
                                               int idx, int n){
  
  Rcpp::NumericVector intensity(n);
  Rcpp::NumericVector mz(n);
  Rcpp::StringVector annotation(n);
  
  //std::list<std::string> annotationT({"y1^1", "y1^2", "y1^3", "b1^1", "b1^2", "b1^3", "y2^1", "y2^2", "y2^3", "b2^1", "b2^2", "b2^3", "y3^1", "y3^2", "y3^3", "b3^1", "b3^2", "b3^3", "y4^1", "y4^2", "y4^3", "b4^1", "b4^2", "b4^3", "y5^1", "y5^2", "y5^3", "b5^1", "b5^2", "b5^3", "y6^1", "y6^2", "y6^3", "b6^1", "b6^2", "b6^3", "y7^1", "y7^2", "y7^3", "b7^1", "b7^2", "b7^3", "y8^1", "y8^2", "y8^3", "b8^1", "b8^2", "b8^3", "y9^1", "y9^2", "y9^3", "b9^1", "b9^2", "b9^3", "y10^1", "y10^2", "y10^3", "b10^1", "b10^2", "b10^3", "y11^1", "y11^2", "y11^3", "b11^1", "b11^2", "b11^3", "y12^1", "y12^2", "y12^3", "b12^1", "b12^2", "b12^3", "y13^1", "y13^2", "y13^3", "b13^1", "b13^2", "b13^3", "y14^1", "y14^2", "y14^3", "b14^1", "b14^2", "b14^3", "y15^1", "y15^2", "y15^3", "b15^1", "b15^2", "b15^3", "y16^1", "y16^2", "y16^3", "b16^1", "b16^2", "b16^3", "y17^1", "y17^2", "y17^3", "b17^1", "b17^2", "b17^3", "y18^1", "y18^2", "y18^3", "b18^1", "b18^2", "b18^3", "y19^1", "y19^2", "y19^3", "b19^1", "b19^2", "b19^3", "y20^1", "y20^2", "y20^3", "b20^1", "b20^2", "b20^3", "y21^1", "y21^2", "y21^3", "b21^1", "b21^2", "b21^3", "y22^1", "y22^2", "y22^3", "b22^1", "b22^2", "b22^3", "y23^1", "y23^2", "y23^3", "b23^1", "b23^2", "b23^3", "y24^1", "y24^2", "y24^3", "b24^1", "b24^2", "b24^3", "y25^1", "y25^2", "y25^3", "b25^1", "b25^2", "b25^3", "y26^1", "y26^2", "y26^3", "b26^1", "b26^2", "b26^3", "y27^1", "y27^2", "y27^3", "b27^1", "b27^2", "b27^3", "y28^1", "y28^2", "y28^3", "b28^1", "b28^2", "b28^3", "y29^1", "y29^2", "y29^3", "b29^1", "b29^2", "b29^3"});
  //Rcpp::StringVector annotationTT(annotation.size());
  //annotationTT = annotationT;
  
  // for debug only
  // Rcpp::NumericVector vidx(n);
  for (int i = 0; i < n; i++) {

    if (output1_data[idx] < 0.0){
      intensity[i] = NA_REAL;
      mz[i] = NA_REAL;
    }else{
      intensity[i] = output0_data[idx];
      mz[i] = output1_data[idx];
    }
    annotation[i] = output2_data[idx];
    idx++;
  }
  
  Rcpp::DataFrame df = Rcpp::DataFrame::create(
    //Rcpp::Named("idx") = vidx,
    Rcpp::Named("annotation") = annotation,
    Rcpp::Named("intensity") = intensity,
    Rcpp::Named("mz") = mz);
  
  return df;
}
//' prosit2019Intensity
//'
//' This function returns a predicted fragment ion spectrum
//' of a given peptide sequence.
//'
//' @inheritParams alphaPeptMs2GenericEnsemble
//' @params usesll usessl default is set to false
//'
//' @examples
//'
//'  dlomix::prosit2019Intensity("LKEATIQLDELNQK",
//'    collisionEnergy = 25, precursorCharge = 2L, verbose = TRUE,
//'    url = "dlomix.fgcz.uzh.ch:8080") |> head()
//' 
//' @export
// [[Rcpp::export]]
Rcpp::List prosit2019Intensity(
  Rcpp::StringVector peptide,
  Rcpp::NumericVector collisionEnergy,
  Rcpp::NumericVector precursorCharge,
  bool verbose = false,
  bool usessl = false,
  std::string url = "dlomix.fgcz.uzh.ch:8080")
{
  long int batch_size = 5000;

  if (peptide.size() > batch_size){
    Rcpp::Rcerr << "number of input peptide is limited to a batch size of " << batch_size << "." << std::endl;
    return(NULL);
  }
  
  Rcpp::List Lrv(peptide.size());
  
  tc::Headers http_headers;
  uint32_t client_timeout = 0;
  bool use_ssl = usessl;
  tc::SslOptions ssl_options;
  tc::KeepAliveOptions keepalive_options;

  std::string model_name = "Prosit_2019_intensity";
  std::string model_version = "";

  // Create a InferenceServerGrpcClient instance to communicate with the
  // server using gRPC protocol.
  std::unique_ptr<tc::InferenceServerGrpcClient> client;
  FAIL_IF_ERR(
      tc::InferenceServerGrpcClient::Create(
          &client, url, verbose, use_ssl, ssl_options, keepalive_options),
      "unable to create grpc client");

  // Create the data for the input tensors. 
  std::vector<std::string> input0_data(peptide.size());
  std::vector<int32_t> input1_data(peptide.size());
  std::vector<float> input2_data(peptide.size());
 
  //Rcpp::Rcout << "pep\t=\t" << peptide[0] << std::endl;

  for (auto i = 0; i < batch_size && i < peptide.size(); ++i) {
   // if (peptide[i].size() != peptide[0].size()){
   //   Rcpp::Rcerr << "number of amino acids of "<<  peptide[i] << " (element "<< i << ") should be " << peptide[0].size() << " as the first peptide in the vector." << std::endl;
    //}
    input0_data[i] = peptide[i];

    if (precursorCharge.size() == 1){
    	input1_data[i] = precursorCharge[0];
    }else{
    	input1_data[i] = precursorCharge[i];
    }
    
    if (collisionEnergy.size() == 1){
    	input2_data[i] = collisionEnergy[0];
    }else{
    	input2_data[i] = collisionEnergy[i];
    }
  }

  std::vector<int64_t> shape{peptide.size(), 1};

  // Initialize the inputs with the data.
  tc::InferInput* input0;
  tc::InferInput* input1;
  tc::InferInput* input2;

// create
  FAIL_IF_ERR(
      tc::InferInput::Create(&input0, "peptide_sequences", shape, "BYTES"),
      "unable to get INPUT0");
  std::shared_ptr<tc::InferInput> input0_ptr;
  input0_ptr.reset(input0);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input1, "precursor_charges", shape, "INT32"),
      "unable to get precursor_charge_in_int:0");
  std::shared_ptr<tc::InferInput> input1_ptr;
  input1_ptr.reset(input1);

  FAIL_IF_ERR(
      tc::InferInput::Create(&input2, "collision_energies", shape, "FP32"),
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
  tc::InferRequestedOutput* output2;

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output0, "intensities"),
      "unable to get 'OUTPUT intensities'");
  std::shared_ptr<tc::InferRequestedOutput> output0_ptr;
  output0_ptr.reset(output0);

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output1, "mz"),
      "unable to get 'OUTPUT mz'");
  std::shared_ptr<tc::InferRequestedOutput> output1_ptr;
  output1_ptr.reset(output1);

  FAIL_IF_ERR(
      tc::InferRequestedOutput::Create(&output2, "annotation"),
      "unable to get 'OUTPUT annotation'");
  std::shared_ptr<tc::InferRequestedOutput> output2_ptr;
  output2_ptr.reset(output2);


  // The inference settings. Will be using default for now.
  tc::InferOptions options(model_name);
  options.model_version_ = model_version;
  options.client_timeout_ = client_timeout;

  std::vector<tc::InferInput*> inputs = {
	  input0_ptr.get(),
  	  input1_ptr.get(),
	  input2_ptr.get()};

  std::vector<const tc::InferRequestedOutput*> outputs = {
	  output0_ptr.get(),
	  output1_ptr.get(),
	  output2_ptr.get()};

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
    results_ptr->RawData("intensities",
      (const uint8_t**)&output0_data, &output0_byte_size),
      "unable to get result data for 'mz'");  

  float* output1_data;
  size_t output1_byte_size;
  FAIL_IF_ERR(
    results_ptr->RawData("mz",
      (const uint8_t**)&output1_data, &output1_byte_size),
      "unable to get result data for 'intensities'");  

  std::vector<std::string> output2_data;
  size_t output2_byte_size;
  FAIL_IF_ERR(
    results_ptr->StringData("annotation", &output2_data), 
      "unable to get result data for 'annotation'");  
  //Rcpp::Rcout << "output0_byte_size\t=\t" << output0_byte_size<< std::endl;
      // #, &output2_byte_size),
 
  int outn = output1_byte_size / 4 / peptide.size();
  
 // Rcpp::Rcout << "\nDEBUG\t" << output2_data.at(0) << std::endl;

 for (int i = 0; i < peptide.size(); i++){
   // Rcpp::Rcout << peptide[i] << "\t" << peptide[i].size() << std::endl;
   Rcpp::DataFrame df = extractPrositPredictedSpectrum(output0_data,
                                                       output1_data,
                                                       output2_data,
                                                       i * outn, 6 * (peptide[i].size() - 1));
   Lrv[i] = df;
 }
 
 
  return Lrv;
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
