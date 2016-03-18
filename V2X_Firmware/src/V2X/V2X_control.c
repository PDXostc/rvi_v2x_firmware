/*
 * V2X_command.c
 *
 * Created: 3/15/2016 9:46:58 AM
 *  Author: jbanks2
 */ 

#include "V2X.h"

void CTL_add_to_buffer(buff * buffer, Bool in_out, char value) {
	switch (in_out) {
	case BUFFER_IN:
		switch (buffer->active_in) {
		case BUFFER_A:
			buffer->input_a[buffer->in_store_a++] = value;
			break;
		case BUFFER_B:
		default:
			buffer->input_b[buffer->in_store_b++] = value;
			break;
		}
		break;
	case BUFFER_OUT:
	default:
		switch (buffer->active_out) {
		case BUFFER_A:
			buffer->output_a[buffer->out_store_a++] = value;
			break;
		case BUFFER_B:
		default:
			buffer->output_b[buffer->out_store_b++] = value;
			break;
		}
		break;
	}
}

void CTL_add_string_to_buffer_P(buff * buffer, Bool in_out, const char * to_add) {
	int len = strlen_P(to_add);
	while (len != 0) {
		CTL_add_to_buffer(buffer, in_out, pgm_read_byte(to_add++));
		len--;
	}
}

void CTL_add_string_to_buffer(buff * buffer, Bool in_out, char * to_add) {
	int len = strlen(to_add);
	while (len != 0) {
		CTL_add_to_buffer(buffer, in_out, to_add[0]);
		to_add++;
		len--;
	}
}

int CTL_bytes_to_send (buff * buffer, Bool in_out) {
	//because the bytes_to_send is a processing specific
	//duty the role of buffer->active_xx is reversed.
	//being active indicates where new bytes are stored
	//this needs to return the opposite
	switch (in_out) { 
		case BUFFER_IN:
			switch (buffer->active_in) {
			case BUFFER_A:  
				return strlen((buffer->input_b+buffer->in_proc_b));
				break;
			case BUFFER_B:
			default:
				return strlen((buffer->input_a+buffer->in_proc_a));
				break;
			}
		break;
		case BUFFER_OUT:
		default:
			switch (buffer->active_out) {
			case BUFFER_A:
				return strlen((buffer->output_b+buffer->out_proc_b));
				break;
			case BUFFER_B:
			default:
				return strlen((buffer->output_a+buffer->out_proc_a));
				break;
			}
		break;
	}
}

char CTL_next_byte (buff * buffer, Bool in_out) {
	//because the bytes_to_send is a processing specific
	//duty the role of buffer->active_xx is reversed.
	//being active indicates where new bytes are stored
	//this needs to return the opposite
	switch (in_out) { 
		case BUFFER_IN:
			switch (buffer->active_in) {
			case BUFFER_A:  
				return buffer->input_b[buffer->in_proc_b++];
				break;
			case BUFFER_B:
			default:
				return buffer->input_a[buffer->in_proc_a++];
				break;
			}
		break;
		case BUFFER_OUT:
		default:
			switch (buffer->active_out) {
			case BUFFER_A:
				return buffer->output_b[buffer->out_proc_b++];
				break;
			case BUFFER_B:
			default:
				return buffer->output_a[buffer->out_proc_a++];
				break;
			}
		break;
	}
}

void CTL_purge_buffer(buff * buffer, Bool in_out) {
	//because the CTL_purge_buffer is a processing specific
	//duty the role of buffer->active_xx is reversed.
	//being active indicates where new bytes are stored
	//this needs to clear the inactive buffer
	switch (in_out) {
		case BUFFER_IN:
			switch (buffer->active_in) {
				case BUFFER_A:
					clear_buffer(buffer->input_b);
					buffer->in_proc_b = 0;
					buffer->in_store_b = 0;
					break;
				case BUFFER_B:
				default:
					clear_buffer(buffer->input_a);
					buffer->in_proc_a = 0;
					buffer->in_store_a = 0;
					break;
				}
			break;
		case BUFFER_OUT:
		default:
			switch (buffer->active_out) {
				case BUFFER_A:
					clear_buffer(buffer->output_b);
					buffer->out_proc_b = 0;
					buffer->out_store_b = 0;
					break;
				case BUFFER_B:
				default:
					clear_buffer(buffer->output_a);
					buffer->out_proc_a = 0;
					buffer->out_store_a = 0;
					break;
				}
			break;
	}
}


void CTL_mark_for_processing(buff * buffer, Bool in_out) {
	switch (in_out) {
		case BUFFER_IN:
		switch (buffer->active_in) {
			case BUFFER_A:
			//is at pointer at least one byte in this buffer
			if (buffer->in_store_a != 0) {
				//see if process pointer is reset to 0
				//has buffer had completed its operation
				if (buffer->in_proc_b != 0) {
					usb_tx_string_P(PSTR("ERROR: input B did not complete"));
					CTL_purge_buffer(buffer, BUFFER_IN);
				}
				//change the active buffer flag
				buffer->active_in = BUFFER_B;
			}//else string was empty, do nothing
			break;
			case BUFFER_B:
			default:
			if (buffer->in_store_b != 0) {
				if (buffer->in_proc_a != 0) {
					usb_tx_string_P(PSTR("ERROR: input A did not complete***"));
					CTL_purge_buffer(buffer, BUFFER_IN);
				}
				buffer->active_in = BUFFER_A;
			}
			break;
		}
		break;
		case BUFFER_OUT:
		default:
		switch (buffer->active_out) {
			case BUFFER_A:
			if (buffer->out_store_a != 0) {
				if (buffer->out_proc_b != 0) {
					usb_tx_string_P(PSTR("ERROR: output B did not complete***"));
					CTL_purge_buffer(buffer, BUFFER_OUT);
				}
				buffer->active_out = BUFFER_B;
			} 
			break;
			case BUFFER_B:
			default:
			if (buffer->out_store_b != 0) {
				if (buffer->out_proc_a != 0) {
					usb_tx_string_P(PSTR("ERROR: output A did not complete***"));
					CTL_purge_buffer(buffer, BUFFER_OUT);
				}
				buffer->active_out = BUFFER_A;
			} 
			break;
		}
		break;
	}
}

void clear_buffer(char * buffer) {
	int cnt = strlen(buffer);
	for(int i = 0; i < cnt; i++)
	{buffer[i] = '\0';}
}

char * CTL_ptr_to_proc_buffer(buff * buffer, Bool in_out) {
	//because the CTL_purge_buffer is a processing specific
	//duty the role of buffer->active_xx is reversed.
	//being active indicates where new bytes are stored
	//this needs to point to the inactive buffer
	switch (in_out) {
	case BUFFER_IN:
		switch (buffer->active_in) {
			case BUFFER_A:
				return (*buffer).input_b;
				break;
			case BUFFER_B:
			default:
				return (*buffer).input_a;
				break;
		}
		break;
	case BUFFER_OUT:
	default:
		switch (buffer->active_out) {
			case BUFFER_A:
				return (*buffer).output_b;
				break;
			case BUFFER_B:
			default:
				return (*buffer).output_a;
				break;
		}
		break;
	}
}

/*	switch (in_out) {
		case BUFFER_IN:
			switch (buffer->active_in) {
				case BUFFER_A:
					break;
				case BUFFER_B:
					break;
			}
			break;
		case BUFFER_OUT:
			switch (buffer->active_out) {
				case BUFFER_A:
					break;
				case BUFFER_B:
					break;
			}
			break;
	} */
