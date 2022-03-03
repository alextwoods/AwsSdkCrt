#include <Rcpp.h>
#include <aws/common/error.h>
using namespace Rcpp;

// [[Rcpp::export]]
String rcpp_aws_error_debug_str(int err) { 
    return String( aws_error_debug_str(err) );
}

// [[Rcpp::export]]
String rcpp_aws_error_name(int err) { 
    return String( aws_error_name(err) );
}

// [[Rcpp::export]]
String rcpp_aws_error_str(int err) { 
    return String( aws_error_str(err) );
}

// [[Rcpp::export]]
int rcpp_aws_last_error() { 
    return aws_last_error();
}

// [[Rcpp::export]]
int rcpp_aws_test_error(int err) { 
    return aws_raise_error(err);
}
