echo "42 is nice" | ./ft_ssl md5
echo "42 is nice" | ./ft_ssl md5 -p
echo "Pity the living." | ./ft_ssl md5 -q -r
./ft_ssl md5 file
./ft_ssl md5 -r file
./ft_ssl md5 -s "pity those that aren't following baerista on spotify."
echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p file
echo "some of this will not make sense at first" | ./ft_ssl md5 file
echo "but eventually you will understand" | ./ft_ssl md5 -p -r file
echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" file
echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar"
echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" file