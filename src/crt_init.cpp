#include <Rcpp.h>
// #include <inttypes.h>
// #include <stddef.h>
// #include <stdbool.h>
// #include <api.h>

#include <aws/common/error.h>


#include <aws/auth/auth.h>
#include <aws/cal/cal.h>
#include <aws/compression/compression.h>
#include <aws/http/http.h>

#include <aws/common/environment.h>
#include <aws/common/string.h>

struct aws_allocator *s_crt_allocator = NULL;

struct aws_allocator *init_allocator(void) {
    /* Default to a small block allocator in front of the CRT default allocator */
    s_crt_allocator = aws_small_block_allocator_new(aws_default_allocator(), true);

    /* Check to see if the user has requested memory tracing */
    enum aws_mem_trace_level trace_level = AWS_MEMTRACE_NONE;
    struct aws_string *mem_tracing_key = aws_string_new_from_c_str(s_crt_allocator, "AWS_CRT_MEMORY_TRACING");
    struct aws_string *mem_tracing_value = NULL;
    if (aws_get_environment_value(s_crt_allocator, mem_tracing_key, &mem_tracing_value) == AWS_OP_SUCCESS &&
        mem_tracing_value != NULL) {
        int tracing_value = (int)strtol((const char *)aws_string_bytes(mem_tracing_value), NULL, 10);
        if (tracing_value < 0 || tracing_value > AWS_MEMTRACE_STACKS) {
            tracing_value = 0;
        }
        trace_level = (enum aws_mem_trace_level)tracing_value;
    }
    aws_string_destroy(mem_tracing_key);
    aws_string_destroy(mem_tracing_value);

    s_crt_allocator = aws_mem_tracer_new(s_crt_allocator, NULL, trace_level, 16);
    return s_crt_allocator;
}

void shutdown_allocator(void) {
    /* destroy/unwrap traced allocator, then destroy it */
    s_crt_allocator = aws_mem_tracer_destroy(s_crt_allocator);
    /* If there are leaks (e.g. OPENSSL), shutting down the allocator will crash */
    if (aws_small_block_allocator_bytes_active(s_crt_allocator)) {
        return;
    }
    aws_small_block_allocator_destroy(s_crt_allocator);
    s_crt_allocator = NULL;
}

aws_allocator *aws_crt_default_allocator(void) {
    return s_crt_allocator;
}

void aws_crt_init(void) {
    init_allocator();
    aws_common_library_init(aws_default_allocator());
    aws_cal_library_init(aws_default_allocator());
    aws_io_library_init(aws_default_allocator());
    // aws_compression_library_init(aws_default_allocator());
    // aws_http_library_init(aws_default_allocator());
    // aws_auth_library_init(aws_default_allocator());

    // aws_crt_log_init();
}

void aws_crt_clean_up(void) {
    // aws_auth_library_clean_up();
    // aws_http_library_clean_up();
    // aws_compression_library_clean_up();
    aws_io_library_clean_up();
    aws_cal_library_clean_up();
    aws_common_library_clean_up();
    shutdown_allocator();
}


using namespace Rcpp;

// [[Rcpp::export]]
void rcpp_aws_crt_init(){
   aws_crt_init();
   return;
}


// [[Rcpp::export]]
int awshc_test_error(int error){
   return aws_raise_error(error);
}

// [[Rcpp::export]]
String awshc_error_string(int error){

  const char* errorCstr = aws_error_name(error);

   return String(errorCstr);
}
