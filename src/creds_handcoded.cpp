#include <crt.h>
#include <credentials.h>
#include <api.h>
#include <Rcpp.h>

class CredentialsOptions; // fwd declarations

RCPP_EXPOSED_CLASS(CredentialsOptions);

class CredentialsOptions {
public:
  CredentialsOptions(Rcpp::StringVector akid, Rcpp::StringVector secretKey) : native(aws_crt_credentials_options_new()) {
    std::string temp_akid = Rcpp::as<std::string>(akid[0]);
    aws_crt_credentials_options_set_access_key_id(native, (const uint8_t*)temp_akid.c_str(), temp_akid.size());

    std::string temp_sk = Rcpp::as<std::string>(secretKey[0]);
    aws_crt_credentials_options_set_secret_access_key(native, (const uint8_t*)temp_sk.c_str(), temp_sk.size());
  }
  ~CredentialsOptions() {
    aws_crt_credentials_options_release(native);
  }

  Rcpp::XPtr<aws_crt_credentials_options> native;

  Rcpp::String getAccessKeyId() {
    return Rcpp::String( (const char*) native->access_key_id.buffer );
  }

  Rcpp::String getSecretKey() {
    return Rcpp::String( (const char*) native->secret_access_key.buffer );
  }
};

RCPP_MODULE(CrtModule){
  using namespace Rcpp;
  class_<CredentialsOptions>("CredentialsOptions")
  .constructor<StringVector, StringVector>()
  .field( "native", &CredentialsOptions::native )
  .method( "getAccessKeyId", &CredentialsOptions::getAccessKeyId )
  .method( "getSecretKey", &CredentialsOptions::getSecretKey )
  ;
};
