MRuby::Gem::Specification.new('mruby-miniz') do |spec|
  spec.license = 'MIT'
  spec.authors = 'scalone'
  spec.version = '1.0.0'
  spec.summary = 'Ruby interface for Miniz v1.15, a zlib-subset to deflate/inflate and zip/unzip without dir support'


  spec.add_dependency('mruby-pack')
  spec.add_dependency('mruby-io')
  spec.add_dependency('mruby-dir')

  spec.cc.include_paths << "#{build.root}/src"
end

