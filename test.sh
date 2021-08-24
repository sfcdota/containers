make fclean
make FT=1 all
echo "test ft containers with running 10000 times"
make clean
time ./containers 1000 > ft_result
make fclean

make STD=1 all
echo "test std containers with running 10000 times"
make clean
time ./containers 1000 > std_result
make fclean

diff ft_result std_result && rm ft_result std_result