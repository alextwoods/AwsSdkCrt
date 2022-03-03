#include <Rcpp.h>
#include <aws/checksums/crc.h>
using namespace Rcpp;

// [[Rcpp::export]]
uint32_t rcpp_aws_crc32(StringVector input, uint32_t previous) { 
    std::string temp_input = Rcpp::as<std::string>(input[0]);
    return aws_checksums_crc32((const uint8_t*)temp_input.c_str(), temp_input.size(), previous);
}

// [[Rcpp::export]]
uint32_t rcpp_aws_crc32c(StringVector input, uint32_t previous) { 
    std::string temp_input = Rcpp::as<std::string>(input[0]);
    return aws_checksums_crc32c((const uint8_t*)temp_input.c_str(), temp_input.size(), previous);
}
