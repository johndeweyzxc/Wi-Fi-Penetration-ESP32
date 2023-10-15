/*
 * @file frame_bypasser_interface.h
 * @author johndeweyzxc (johndewey02003@gmail.com)
 * @brief Provides interface for injecting arbitrary 802.11 frames
 */

#pragma once

/*
 * @brief Injects raw arbitrary 802.11 frame
 * @param *frame_buffer Buffer that contains the frame to be injected
 * @param size Size of the frame buffer
 */
void frame_bypasser_inject_frame(uint8_t *frame_buffer, int size);