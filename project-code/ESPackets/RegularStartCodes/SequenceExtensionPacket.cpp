//
// Created by elnsa on 2019-12-29.
//

#include "SequenceExtensionPacket.h"

SequenceExtensionPacket::SequenceExtensionPacket(SequenceExtensionPacket::initializerStruct init) {
    e_type = init.e_type;
    profile_and_level_indication = init.profile_and_level_indication;
    progressive_sequence = init.progressive_sequence;
    chroma_format = init.chroma_format;
    horizontal_size_extension = init.horizontal_size_extension;
    vertical_size_extension = init.vertical_size_extension;
    bit_rate_extension = init.bit_rate_extension;
    vbv_buffer_size_extension = init.vbv_buffer_size_extension;
    low_delay = init.low_delay;
    frame_rate_extension_n = init.frame_rate_extension_n;
    frame_rate_extension_d = init.frame_rate_extension_d;
}

void SequenceExtensionPacket::print() {
    std::printf(
            "SequenceExtensionPacket: identifier %x, pli %x, ps %x, cf %x, hse %x, vse %x, bre %x, vbv_buf_e %x, ld %x, fren %x, fred %x.\n",
            e_type, profile_and_level_indication, progressive_sequence, chroma_format,
            horizontal_size_extension, vertical_size_extension, bit_rate_extension, vbv_buffer_size_extension,
            low_delay, frame_rate_extension_n, frame_rate_extension_d);
}

bool SequenceExtensionPacket::operator==(const SequenceExtensionPacket &rhs) const {
    return e_type == rhs.e_type &&
           profile_and_level_indication == rhs.profile_and_level_indication &&
           progressive_sequence == rhs.progressive_sequence &&
           chroma_format == rhs.chroma_format &&
           horizontal_size_extension == rhs.horizontal_size_extension &&
           vertical_size_extension == rhs.vertical_size_extension &&
           bit_rate_extension == rhs.bit_rate_extension &&
           vbv_buffer_size_extension == rhs.vbv_buffer_size_extension &&
           low_delay == rhs.low_delay &&
           frame_rate_extension_n == rhs.frame_rate_extension_n &&
           frame_rate_extension_d == rhs.frame_rate_extension_d;
}

bool SequenceExtensionPacket::operator!=(const SequenceExtensionPacket &rhs) const {
    return !(rhs == *this);
}