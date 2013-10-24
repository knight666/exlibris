# systembuilder.py
#
# script for converting a tga image to a series of
# glyph bitmaps encoded into 12 bytes

import struct


glyphs = [
	{ 'x':0,   'y':0,  'codepoint':' ' },
	{ 'x':9,   'y':0,  'codepoint':'!' },
	{ 'x':18,  'y':0,  'codepoint':'"' },
	{ 'x':27,  'y':0,  'codepoint':'#' },
	{ 'x':36,  'y':0,  'codepoint':'$' },
	{ 'x':45,  'y':0,  'codepoint':'%' },
	{ 'x':54,  'y':0,  'codepoint':'&' },
	{ 'x':63,  'y':0,  'codepoint':'\'' },
	{ 'x':72,  'y':0,  'codepoint':'(' },
	{ 'x':81,  'y':0,  'codepoint':')' },
	{ 'x':90,  'y':0,  'codepoint':'*' },
	{ 'x':99,  'y':0,  'codepoint':'+' },
	{ 'x':108, 'y':0,  'codepoint':',' },
	{ 'x':117, 'y':0,  'codepoint':'-' },
	{ 'x':126, 'y':0,  'codepoint':'.' },
	{ 'x':135, 'y':0,  'codepoint':'/' },
	{ 'x':144, 'y':0,  'codepoint':'0' },
	{ 'x':153, 'y':0,  'codepoint':'1' },
	{ 'x':162, 'y':0,  'codepoint':'2' },
	{ 'x':171, 'y':0,  'codepoint':'3' },
	{ 'x':180, 'y':0,  'codepoint':'4' },
	{ 'x':189, 'y':0,  'codepoint':'5' },
	{ 'x':198, 'y':0,  'codepoint':'6' },
	{ 'x':207, 'y':0,  'codepoint':'7' },
	{ 'x':216, 'y':0,  'codepoint':'8' },
	{ 'x':225, 'y':0,  'codepoint':'9' },
	{ 'x':234, 'y':0,  'codepoint':':' },
	{ 'x':243, 'y':0,  'codepoint':';' },
	{ 'x':252, 'y':0,  'codepoint':'<' },
	{ 'x':261, 'y':0,  'codepoint':'=' },
	{ 'x':270, 'y':0,  'codepoint':'>' },
	{ 'x':279, 'y':0,  'codepoint':'?' },
	{ 'x':0,   'y':13, 'codepoint':'@' },
	{ 'x':9,   'y':13, 'codepoint':'A' },
	{ 'x':18,  'y':13, 'codepoint':'B' },
	{ 'x':27,  'y':13, 'codepoint':'C' },
	{ 'x':36,  'y':13, 'codepoint':'D' },
	{ 'x':45,  'y':13, 'codepoint':'E' },
	{ 'x':54,  'y':13, 'codepoint':'F' },
	{ 'x':63,  'y':13, 'codepoint':'G' },
	{ 'x':72,  'y':13, 'codepoint':'H' },
	{ 'x':81,  'y':13, 'codepoint':'I' },
	{ 'x':90,  'y':13, 'codepoint':'J' },
	{ 'x':99,  'y':13, 'codepoint':'K' },
	{ 'x':108, 'y':13, 'codepoint':'L' },
	{ 'x':117, 'y':13, 'codepoint':'M' },
	{ 'x':126, 'y':13, 'codepoint':'N' },
	{ 'x':135, 'y':13, 'codepoint':'O' },
	{ 'x':144, 'y':13, 'codepoint':'P' },
	{ 'x':153, 'y':13, 'codepoint':'Q' },
	{ 'x':162, 'y':13, 'codepoint':'R' },
	{ 'x':171, 'y':13, 'codepoint':'S' },
	{ 'x':180, 'y':13, 'codepoint':'T' },
	{ 'x':189, 'y':13, 'codepoint':'U' },
	{ 'x':198, 'y':13, 'codepoint':'V' },
	{ 'x':207, 'y':13, 'codepoint':'W' },
	{ 'x':216, 'y':13, 'codepoint':'X' },
	{ 'x':225, 'y':13, 'codepoint':'Y' },
	{ 'x':234, 'y':13, 'codepoint':'Z' },
	{ 'x':243, 'y':13, 'codepoint':'[' },
	{ 'x':252, 'y':13, 'codepoint':'\\' },
	{ 'x':261, 'y':13, 'codepoint':']' },
	{ 'x':270, 'y':13, 'codepoint':'^' },
	{ 'x':279, 'y':13, 'codepoint':'_' },
	{ 'x':0,   'y':26, 'codepoint':'`' },
	{ 'x':9,   'y':26, 'codepoint':'a' },
	{ 'x':18,  'y':26, 'codepoint':'b' },
	{ 'x':27,  'y':26, 'codepoint':'c' },
	{ 'x':36,  'y':26, 'codepoint':'d' },
	{ 'x':45,  'y':26, 'codepoint':'e' },
	{ 'x':54,  'y':26, 'codepoint':'f' },
	{ 'x':63,  'y':26, 'codepoint':'g' },
	{ 'x':72,  'y':26, 'codepoint':'h' },
	{ 'x':81,  'y':26, 'codepoint':'i' },
	{ 'x':90,  'y':26, 'codepoint':'j' },
	{ 'x':99,  'y':26, 'codepoint':'k' },
	{ 'x':108, 'y':26, 'codepoint':'l' },
	{ 'x':117, 'y':26, 'codepoint':'m' },
	{ 'x':126, 'y':26, 'codepoint':'n' },
	{ 'x':135, 'y':26, 'codepoint':'o' },
	{ 'x':144, 'y':26, 'codepoint':'p' },
	{ 'x':153, 'y':26, 'codepoint':'q' },
	{ 'x':162, 'y':26, 'codepoint':'r' },
	{ 'x':171, 'y':26, 'codepoint':'s' },
	{ 'x':180, 'y':26, 'codepoint':'t' },
	{ 'x':189, 'y':26, 'codepoint':'u' },
	{ 'x':198, 'y':26, 'codepoint':'v' },
	{ 'x':207, 'y':26, 'codepoint':'w' },
	{ 'x':216, 'y':26, 'codepoint':'x' },
	{ 'x':225, 'y':26, 'codepoint':'y' },
	{ 'x':234, 'y':26, 'codepoint':'z' },
	{ 'x':243, 'y':26, 'codepoint':'{' },
	{ 'x':252, 'y':26, 'codepoint':'|' },
	{ 'x':261, 'y':26, 'codepoint':'}' },
	{ 'x':270, 'y':26, 'codepoint':'~' },
	{ 'x':279, 'y':26, 'codepoint':'invalid' },
]

print 'Loading systemfont.tga...'

file = open('systemfont.tga', 'rb')

header = {
	"id": struct.unpack('B', file.read(1))[0],
	"colormap": struct.unpack('B', file.read(1))[0],
	"compression": struct.unpack('B', file.read(1))[0],
	"colormap_offset": struct.unpack('H', file.read(2))[0],
	"colormap_length": struct.unpack('H', file.read(2))[0],
	"colormap_bitsperpixel": struct.unpack('B', file.read(1))[0],
	"origin_x": struct.unpack('H', file.read(2))[0],
	"origin_y": struct.unpack('H', file.read(2))[0],
	"width": struct.unpack('H', file.read(2))[0],
	"height": struct.unpack('H', file.read(2))[0],
	"bits_per_pixel": struct.unpack('B', file.read(1))[0],
	"image_descriptor": struct.unpack('B', file.read(1))[0],
}

if header["compression"] <> 2:
	print "ERROR: Image is not uncompressed."
	exit(1)

if header["bits_per_pixel"] <> 32:
	print "ERROR: Image does not have a bitdepth of 32."
	exit(1)

src_pitch = header["width"] * 4
src = file.read(header["height"] * src_pitch)

def EncodeGlyph(_x, _y):
	glyph_x = _x
	glyph_y = (header["height"] - 1) - _y

	encoded_upper = 0
	encoded_middle = 0
	encoded_lower = 0
	encoded_bit = 31

	for y in reversed(xrange(glyph_y - 3, glyph_y + 1)):
		upper_line_start = y * src_pitch
		upper_line_end = upper_line_start + src_pitch
		upper_line = src[upper_line_start:upper_line_end]
		
		middle_line_start = (y - 4) * src_pitch
		middle_line_end = middle_line_start + src_pitch
		middle_line = src[middle_line_start:middle_line_end]
		
		lower_line_start = (y - 8) * src_pitch
		lower_line_end = lower_line_start + src_pitch
		lower_line = src[lower_line_start:lower_line_end]

		pixel_index = glyph_x * 4
		
		for x in xrange(0, 8):
			upper_alpha = struct.unpack('L', upper_line[pixel_index:pixel_index + 4])[0]
			if (upper_alpha & 0x000000FF) == 0:
				encoded_upper |= (1 << encoded_bit)
			
			middle_alpha = struct.unpack('L', middle_line[pixel_index:pixel_index + 4])[0]
			if (middle_alpha & 0x000000FF) == 0:
				encoded_middle |= (1 << encoded_bit)
				
			lower_alpha = struct.unpack('L', lower_line[pixel_index:pixel_index + 4])[0]
			if (lower_alpha & 0x000000FF) == 0:
				encoded_lower |= (1 << encoded_bit)
			
			#print '0x{0:08X}'.format(upper_alpha), '0x{0:08X}'.format(middle_alpha), '0x{0:08X}'.format(lower_alpha)
			
			encoded_bit -= 1
			pixel_index += 4
	
	return ['0x{0:08X}'.format(encoded_upper), '0x{0:08X}'.format(encoded_middle), '0x{0:08X}'.format(encoded_lower)]

print 'Encoding to systemfont.txt...'
	
output_file = open('systemfont.txt', 'w')
output_file.write('// encoded using systembuilder.py\n')
output_file.write('// for reference, please see systemfont.tga\n')
output_file.write('\n')
output_file.write('static const unsigned int s_GlyphBitmapsEncoded[] = {\n')

for g in glyphs:
	encoded = EncodeGlyph(g['x'], g['y'])
	output_file.write('\t{0}, {1}, {2}, /* {3} */\n'.format(encoded[0], encoded[1], encoded[2], g['codepoint']))

output_file.write('};')

print 'Done.'