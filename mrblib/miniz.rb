class Miniz
  def self.unzip(filezip, path = ".")
    dir_name = filezip.split(".")[-2].split("/").last
    dir = "#{path}/#{dir_name}"
    Dir.mkdir(dir) unless File.exists?(dir)
    self._unzip(filezip, dir)
  end
end

