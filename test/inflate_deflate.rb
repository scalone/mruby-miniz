
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

assert('deflate big') do
  string = "Hello Hello Hello Hello Hello Hello!" * 100
  block = "x\001\355\3121\r\0000\b\000A+\255\e\214\260\221\340\177\303\001;\311-\277\374EV\365\213\255\177\275\f\3030\f\3030\f\3030\314I3\026]\337\201"
  assert_equal block, Miniz.deflate(string)
end
