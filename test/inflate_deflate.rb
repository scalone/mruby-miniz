
assert('deflate') do
  string = "Hello Hello Hello Hello Hello Hello!"
  block = "x\001\363H\315\311\311W\360\300G*\002\000\350\317\fz"
  assert_equal block, Miniz.deflate(string)
end

assert('inflate') do
  string = "Hello Hello Hello Hello Hello Hello!"
  block = "x\001\363H\315\311\311W\360\300G*\002\000\350\317\fz"
  assert_equal string, Miniz.inflate(block)
end
