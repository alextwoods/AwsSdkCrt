
#include <Rcpp.h>

#include <aws/checksums/crc.h>
using namespace Rcpp;

// [[Rcpp::export]]
uint32_t rcpp_handcoded_aws_checksums_crc32(StringVector input, uint32_t previous = 0){

   std::string temp = Rcpp::as<std::string>(input[0]);
   const char* tmp_c_str = temp.c_str();
   return aws_checksums_crc32((const uint8_t*)tmp_c_str, temp.size(), previous);
}


// AWS_CHECKSUMS_API uint32_t aws_checksums_crc32(const uint8_t *input, int length, uint32_t previousCrc32);
