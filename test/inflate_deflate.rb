
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

assert('deflate json') do
  string = "{\"token\":\"374853\",\"id\":\"002\",\"heartbeat\":\"180\"}"
  block = "x\001\253V*\311\317N\315S\262R267\26105V\322Q\312L\001\362\f\f\214\200\314\214\324\304\242\222\244\324\304\022\240\210\241\205\201R-\0001Z\f\236"
  assert_equal block, Miniz.deflate(string)
  assert_equal string, Miniz.inflate(block)
end

assert('deflate small') do
  string = "123456123456"
  block = "x\0013426153\004\223\000\017\232\002k"
  assert_equal block, Miniz.deflate(string)
end
