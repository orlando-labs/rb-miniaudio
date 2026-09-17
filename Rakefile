require 'rake/extensiontask'

GEMSPEC = Gem::Specification.load('miniaudio.gemspec')

Rake::ExtensionTask.new('miniaudio', GEMSPEC) do |ext|
  ext.lib_dir = "lib/miniaudio"
end
