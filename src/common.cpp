#include <Rcpp.h>
#include <api.h>
using namespace Rcpp;

// [[Rcpp::export]]
String aws_error_debug_str(int err) { 
    return String( aws_crt_error_debug_str(err) );
}

// [[Rcpp::export]]
String aws_error_name(int err) { 
    return String( aws_crt_error_name(err) );
}

// [[Rcpp::export]]
String aws_error_str(int err) { 
    return String( aws_crt_error_str(err) );
}

// [[Rcpp::export]]
int aws_last_error() { 
    return aws_crt_last_error();
}

// [[Rcpp::export]]
int aws_test_error(int err) { 
    return aws_crt_test_error(err);
}
