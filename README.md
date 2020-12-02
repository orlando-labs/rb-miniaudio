# Introduction

This is a Ruby wrapper for [Miniaudio library](https://github.com/mackron/miniaudio)

# Installation

## Bundler

Type in your Gemfile

```ruby
gem 'miniaudio'
```

then

```
bundle install
```

Or
```
gem install miniaudio
```

# Usage

## Basic usage

```ruby
require 'miniaudio'

bin_string, sample_rate, num_channels, total_samples = Miniaudio.mp3_read_file_s16 'path/to/file'
```

## Currently available methods

```ruby
Miniaudio.methods.select { |m| m.to_s.include? 'read_' }
 => [:wav_read_s32, :wav_read_f32, :mp3_read_s16, :mp3_read_f32, :flac_read_s16, :flac_read_s32, :flac_read_f32, :wav_read_file_s16, :wav_read_file_s32, :wav_read_file_f32, :mp3_read_file_s16, :mp3_read_file_f32, :flac_read_file_s16, :flac_read_file_s32, :flac_read_file_f32, :vorbis_read_file_s16, :wav_read_s16]
```
