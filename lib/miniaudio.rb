module Miniaudio
  VERSION = '0.3.3'

  FRAME_FORMATS = %i[u8 s16 s24 s32 f32].freeze

  G711_ALGORYTHMS = %i[alaw mulaw].freeze
  G711_ALGOS = G711_ALGORYTHMS
  G711_CONVERTIBLE_FORMATS = %i[s16 s32 f32].freeze

  G711_SAMPLE_RATE = 8000

  class << self
    def convert_frames(
      source_string,
      src_frame_format, src_channels_num, src_sample_rate,
      dst_frame_format, dst_channels_num, dst_sample_rate)

      raise ArgumentError, "Unable to convert to G.711 codec" if G711_ALGOS.include?(dst_frame_format)

      if G711_ALGOS.include?(src_frame_format)
        target_format = FRAME_FORMATS.include?(dst_frame_format) ? dst_frame_format : :s16

        pcm = method("#{src_frame_format}_to_#{target_format}").(source_string)

        if src_sample_rate != dst_sample_rate || src_channels_num != dst_channels_num || dst_frame_format != target_format
          converted, bytes_used = _convert_frames(
            pcm,
            target_format, src_channels_num, G711_SAMPLE_RATE,
            dst_frame_format, dst_channels_num, dst_sample_rate
          )

          [converted, bytes_used * source_string.length / pcm.length]
        else
          [pcm, source_string.length]
        end
      else
        _convert_frames(
          source_string,
          src_frame_format, src_channels_num, src_sample_rate,
          dst_frame_format, dst_channels_num, dst_sample_rate
        )
      end
    end
  end
end

require_relative 'miniaudio/miniaudio'

