//
// Created by elnsa on 2019-12-29.
//

#include "Slice.h"

Slice::Slice(Slice::initializerStruct init) {
    stream_id = init.stream_id;
    packet_type = ESPacket::start_code::slice;
    slice_vertical_position_extension = init.slice_vertical_position_extension;
    quantiser_scale_code = init.quantiser_scale_code;
    slice_extension_flag = init.slice_extension_flag;
    intra_slice = init.intra_slice;
    slice_picture_id_enable = init.slice_picture_id_enable;
    slice_picture_id = init.slice_picture_id;
    numMacroblocks = init.numMacroblocks;
    macroblocks = init.macroblocks;
}

void Slice::print() {
    printf("Slice: id = %hhx, vpe = %hhu, qsc = %hhu, sef = %hhu, is = %hhu, spide = %hhu, spid = %hhu, numMacroblocks = %u\n",
           stream_id, slice_vertical_position_extension, quantiser_scale_code, slice_extension_flag, intra_slice,
           slice_picture_id_enable, slice_picture_id, numMacroblocks);
//    for (size_t i = 0; i < numMacroblocks; i++) {
//        macroblocks[i].print();
//    }
}

bool Slice::operator==(const Slice &rhs) const {
    bool eq = stream_id == rhs.stream_id &&
              packet_type == rhs.packet_type &&
              slice_vertical_position_extension == rhs.slice_vertical_position_extension &&
              quantiser_scale_code == rhs.quantiser_scale_code &&
              slice_extension_flag == rhs.slice_extension_flag &&
              intra_slice == rhs.intra_slice &&
              slice_picture_id_enable == rhs.slice_picture_id &&
              slice_picture_id == rhs.slice_picture_id &&
              numMacroblocks == rhs.numMacroblocks;
    if (!eq)return eq;
    for (int i = 0; i < numMacroblocks; i++) {
        if (macroblocks[i] != rhs.macroblocks[i])return false;
    }
    return true;
}

bool Slice::operator!=(const Slice &rhs) const {
    return !(rhs == *this);
}

Slice::~Slice() {
    for (int i = 0; i < numMacroblocks; i++) {
        delete (&macroblocks[i]);
    }
}