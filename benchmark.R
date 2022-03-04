library(AwsSdkCrt)
library(digest)
library(microbenchmark)

createRandString<- function(len = 50) {
  v = c(sample(LETTERS, len, replace = TRUE),
        sample(0:9, 4, replace = TRUE),
        sample(LETTERS, 1, replace = TRUE))
  return(paste0(v,collapse = ""))
}

rand_strings = sapply(1:100, function(x) createRandString(2000))

mbm <- microbenchmark("digest" = { digest(sample(rand_strings, 1), algo="crc32", serialize=FALSE, raw=TRUE) },
               "crt" = {
                 aws_crc32(sample(rand_strings, 1), 0)
               }, times=10000)

cat("\nBenchmarking completed.  CRC32 on random 2000 character strings, results:\n")
print(mbm)
