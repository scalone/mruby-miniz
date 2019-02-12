class Miniz

  #flush
  MZ_NO_FLUSH            = 0
  MZ_PARTIAL_FLUSH       = 1
  MZ_SYNC_FLUSH          = 2
  MZ_FULL_FLUSH          = 3
  MZ_FINISH              = 4
  MZ_BLOCK               = 5
  MZ_OK                  = 0

  #returns
  MZ_STREAM_END          = 1
  MZ_NEED_DICT           = 2
  MZ_ERRNO               = -1
  MZ_STREAM_ERROR        = -2
  MZ_DATA_ERROR          = -3
  MZ_MEM_ERROR           = -4
  MZ_BUF_ERROR           = -5
  MZ_VERSION_ERROR       = -6
  MZ_PARAM_ERROR         = -10000

  #compression
  MZ_NO_COMPRESSION      = 0
  MZ_BEST_SPEED          = 1
  MZ_BEST_COMPRESSION    = 9
  MZ_UBER_COMPRESSION    = 10
  MZ_DEFAULT_LEVEL       = 6
  MZ_DEFAULT_COMPRESSION = -1

  #strategies
  MZ_DEFAULT_STRATEGY    = 0
  MZ_FILTERED            = 1
  MZ_HUFFMAN_ONLY        = 2
  MZ_RLE                 = 3
  MZ_FIXED               = 4

  MZ_DEFAULT_WINDOW_BITS = 15

  MZ_DEFAULT_MEM_LEVEL   = 8

  def self.unzip(filezip, path = ".")
    dir = "#{path}/#{filezip.split(".")[0]}"
    self._unzip(filezip, dir)
  end

  def self.deflate(string, level = MZ_BEST_COMPRESSION, flush = MZ_SYNC_FLUSH, window_bits = -MZ_DEFAULT_WINDOW_BITS, mem_level = MZ_DEFAULT_MEM_LEVEL, strategy = MZ_FIXED)
    _deflate(string, level, flush, window_bits, mem_level, strategy)
  end

  def self.inflate(block, window_bits = -MZ_DEFAULT_WINDOW_BITS)
    _inflate(block, window_bits)
  end
end

