//
// Created by elnsa on 2019-12-29.
//

#include "PESPacket.h"

#define getName(var)  #var

void PESPacket::print() {
    std::printf("start_code = %s\nstream_id = %hhx\nPES_Packet_length = %hu\nPES_scrambling_control = %hhu\n"
                "PES_priority = %hhu\ndata_alignment_indicator = %hhu\ncopyright = %hhu\noriginal_or_copy = %hhu\n"
                "PTS_DTS_flags = %hhu\nESCR_flag = %hhu\nES_rate_flag = %hhu\nDSM_trick_mode_flag = %hhu\n"
                "additional_copy_info_flag = %hhu\nPES_CRC_flag = %hhu\nPES_extension_flag = %hhu\nPES_header_data_length = %hhu\n"
                "PTS_DTS_Fields:\n\t"
                "PTS = %llu, DTS = %llu\n"
                "ESCR = %llu,\nES_rate = %u\nDSM_Trick_Mode_Fields:\n\t"
                "trick_mode_control = %s, field_id = %hhu, intra_slice_refresh = %hhu, frequency_truncation = %hhu, rep_cntrl = %hhu\n"
                "additional_copy_info = %hhu\nprevious_packet_CRC = %hu\nPES_Extensions_Fields:\n\t"
                "PES_private_data_flag = %hhu, pack_header_field_flag = %hhu, program_packet_sequence_counter_flag = %hhu, "
                "P_STD_buffer_flag = %hhu, PES_extension_flag_2 = %hhu, pack_field_length = %hhu\n"
                "Pack_Header:\n\t\t"
                "system_clock_reference = %llu, program_mux_rate = %u\n"
                "System_Header:\n\t\t\t"
                "header_length = %hu, rate_bound = %u, audio_bound = %hhu, fixed_flag = %hhu, CSPS_flag = %hhu, "
                "system_audio_lock_flag = %hhu, system_video_lock_flag = %hhu, video_bound = %hhu, packet_rate_restriction_flag  = %hhu,"
                "numP_STD = %u\n"
                "P_STD* val = %p\n", getName(this->packet_type), this->stream_id, this->PES_packet_length,
                this->PES_scrambling_control, this->PES_priority, this->data_alignment_indicator, this->copyright,
                this->original_or_copy, this->PTS_DTS_flags, this->ESCR_flag, this->ES_rate_flag, this->DSM_trick_mode_flag,
                this->additional_copy_info_flag, this->PES_CRC_flag, this->PES_extension_flag, this->PES_header_data_length,
                this->pts_dts.PTS, this->pts_dts.DTS, this->ESCR, this->ES_rate, getName(this->dsm_trick_mode.trick_mode_control),
                this->dsm_trick_mode.field_id, this->dsm_trick_mode.intra_slice_refresh, this->dsm_trick_mode.frequency_truncation,
                this->dsm_trick_mode.rep_cntrl, this->additional_copy_info, this->previous_PES_packet_CRC,
                this->pes_extension_fields.PES_private_data_flag, this->pes_extension_fields.pack_header_field_flag,
                this->pes_extension_fields.program_packet_sequence_counter, this->pes_extension_fields.P_STD_buffer_flag,
                this->pes_extension_fields.PES_extension_flag_2, this->pes_extension_fields.pack_field_length,
                this->pes_extension_fields.pack_header.system_clock_reference, this->pes_extension_fields.pack_header.program_mux_rate,
                this->pes_extension_fields.pack_header.system_header.header_length,
                this->pes_extension_fields.pack_header.system_header.rate_bound,
                this->pes_extension_fields.pack_header.system_header.audio_bound,
                this->pes_extension_fields.pack_header.system_header.fixed_flag,
                this->pes_extension_fields.pack_header.system_header.CSPS_flag,
                this->pes_extension_fields.pack_header.system_header.system_audio_lock_flag,
                this->pes_extension_fields.pack_header.system_header.system_video_lock_flag,
                this->pes_extension_fields.pack_header.system_header.video_bound,
                this->pes_extension_fields.pack_header.system_header.packet_rate_restriction_flag,
                this->pes_extension_fields.pack_header.system_header.numPSTD,
                this->pes_extension_fields.pack_header.system_header.p_std);

    for (size_t i = 0; i < this->pes_extension_fields.pack_header.system_header.numPSTD; i++) {
        std::printf("\t\t\t\t P_STD[%u]:\n"
                    "\t\t\t\t\t stream_id = %s, P_STD_buffer_bound_scale = %hhu, P_STD_buffer_size_bound = %hhu\n",
                    i, getName(this->pes_extension_fields.pack_header.system_header.p_std[i].stream_id),
                    i, this->pes_extension_fields.pack_header.system_header.p_std[i].P_STD_buffer_bound_scale,
                    i, this->pes_extension_fields.pack_header.system_header.p_std[i].P_STD_buffer_size_bound);
    }

    std::printf("\t MPEG1_MPEG2_identifier = %hhu, original_stuff_length = %hhu, P_STD_buffer_scale = %hhu, "
                "P_STD_buffer_size = %hu, PES_extension_field_length = %hhu\n data_length = %u\ndata : %p\n\t",
                this->pes_extension_fields.MPEG1_MPEG2_identifier, this->pes_extension_fields.original_stuff_length,
                this->pes_extension_fields.P_STD_buffer_scale, this->pes_extension_fields.P_STD_buffer_size,
                this->pes_extension_fields.PES_extension_field_length, this->data_length, this->data);

    for (size_t i = 0; i < this->data_length; i++) {
        if (i == 0) {
            std::printf("%hhu", data[i]);
        } else {
            std::printf(" %hhu", data[i]);
        }
    }

}


PESPacket::PESPacket(start_code packet_type, unsigned char stream_id, unsigned short PES_packet_length,
                    unsigned char PES_scrambling_control, unsigned char PES_priority, unsigned char data_alignment_indicator, unsigned char copyright, unsigned char original_or_copy,
                     unsigned char PTS_DTS_flags, unsigned char ESCR_flag, unsigned char ES_rate_flag,
                     unsigned char DSM_trick_mode_flag, unsigned char additional_copy_info_flag,
                     unsigned char PES_CRC_flag, unsigned char PES_extension_flag, unsigned char PES_header_data_length,
                     PESPacket::pts_dts_fields pts_dts, unsigned long long ESCR, unsigned int ES_rate,
                     PESPacket::dsm_trick_mode_fields dsm_trick_mode, unsigned char additional_copy_info,
                     unsigned short previous_PES_packet_CRC, PESPacket::PES_extension_fields pes_extension_fields,
                     size_t data_length, unsigned char* data) {
    this->packet_type = packet_type;
    this->stream_id = stream_id;
    this->PES_packet_length = PES_packet_length;
    this->PES_scrambling_control = PES_scrambling_control;
    this->PES_priority = PES_priority;
    this->data_alignment_indicator = data_alignment_indicator;
    this->copyright = copyright;
    this->original_or_copy = original_or_copy;
    this->PTS_DTS_flags = PTS_DTS_flags;
    this->ESCR_flag = ESCR_flag;
    this->ES_rate_flag = ES_rate_flag;
    this->DSM_trick_mode_flag = DSM_trick_mode_flag;
    this->additional_copy_info_flag = additional_copy_info_flag;
    this->PES_CRC_flag = PES_CRC_flag;
    this->PES_extension_flag = PES_extension_flag;
    this->PES_header_data_length = PES_header_data_length;
    this->pts_dts = pts_dts;
    this->ESCR = ESCR;
    this->ES_rate = ES_rate;
    this->dsm_trick_mode = dsm_trick_mode;
    this->additional_copy_info = additional_copy_info;
    this->previous_PES_packet_CRC = previous_PES_packet_CRC;
    this->pes_extension_fields = pes_extension_fields;
    this->data_length = data_length;
    this->data = data;
}