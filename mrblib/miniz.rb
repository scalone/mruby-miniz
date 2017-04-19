class Miniz
  def self.unzip(filezip, path = ".")
    dir = "#{path}/#{filezip.split(".")[0]}"
    self._unzip(filezip, dir)
  end
end

