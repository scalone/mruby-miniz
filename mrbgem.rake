MRuby::Gem::Specification.new('mruby-miniz') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Thiago Scalone'
  spec.version = "1.0.0"

  spec.cc.include_paths << "#{build.root}/src"

  spec.add_dependency('mruby-io')
  spec.add_dependency('mruby-dir')
end
