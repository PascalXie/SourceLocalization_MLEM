rm -r *

cmake ..
make
./alice

cp debug_log_detectorCountingNumbers.txt input.txt
./alice
