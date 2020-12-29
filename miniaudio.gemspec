# frozen_string_literal: true
require 'date'

c_srcs = [
  'ext/miniaudio/rb-miniaudio.c',
  'ext/miniaudio/read_functions.h',
  'ext/miniaudio/convert_functions.h',
  'ext/miniaudio/miniaudio/miniaudio.h',
  'ext/miniaudio/miniaudio/extras/stb_vorbis.c'
]

Gem::Specification.new do |s|
  s.name = 'miniaudio'
  s.required_ruby_version = '>= 2.5.0'
  s.version = '0.3.1'
  s.date = Date.today.to_s
  s.summary = 'Miniaudio library Ruby wrapper'
  s.description = 'Miniaudio library (https://github.com/mackron/miniaudio) Ruby wrapper'
  s.authors = ['Ivan Razuvaev']
  s.email = 'team@orlando-labs.com'
  s.files = %w[lib/miniaudio.rb] + c_srcs
  s.homepage = 'https://github.com/orlando-labs/rb-miniaudio'
  s.license = 'MIT'
  s.extensions = %w[ext/miniaudio/extconf.rb]
end
