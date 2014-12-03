class Miniz
  def self.unzip(filezip)
    dir = "#{filezip.split(".")[0]}"
    clean(dir)
    self._unzip(filezip, dir)
  end

  # TODO Scalone: Refactor file check
  def self.clean(dir)
    if Dir.exist?(dir)
      files = Dir.entries(dir)
      if (files.size > 2)
        files[2..-1].each do |file|
          File.delete("#{dir}/#{file}")
        end
      end
      Dir.delete(dir)
    end
    Dir.mkdir(dir)
  end
end
