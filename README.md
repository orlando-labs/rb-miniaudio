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

bin_string, sample_rate, num_channels, samples_per_channel = Miniaudio.mp3_read_file_s16 'path/to/file'
```

## Currently available methods for reading

```ruby
Miniaudio.methods.select { |m| m.to_s.include? 'read_' }
 => [:wav_read_s32, :wav_read_f32, :mp3_read_s16, :mp3_read_f32, :flac_read_s16, :flac_read_s32, :flac_read_f32, :wav_read_file_s16, :wav_read_file_s32, :wav_read_file_f32, :mp3_read_file_s16, :mp3_read_file_f32, :flac_read_file_s16, :flac_read_file_s32, :flac_read_file_f32, :vorbis_read_file_s16, :wav_read_s16]
```

## Currently available methods for audio format conversion

```ruby
converted, source_bytes_used = Miniaudio.convert_frames 
  source_string, 
  src_frame_format, src_channels_num, src_sample_rate, 
  dst_frame_format, dst_channels_num, dst_sample_rate 
```

Allowed frame formats for src:

```ruby
Miniaudio::FRAME_FORMATS + Miniaudio::G711_ALGOS
 => [:u8, :s16, :s24, :s32, :f32, :alaw, :mulaw]
```

If one of G.711 formats provided then src_sample_rate is forced to be equal 8000.

Allowed frame formats for dst:

```ruby
Miniaudio::FRAME_FORMATS
 => [:u8, :s16, :s24, :s32, :f32]
```
