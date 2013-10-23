import struct

glyphs = [
	{ 'x':125, 'y':39, 'codepoint':' ' },
	{ 'x':90, 'y':26, 'codepoint':'!' },
	{ 'x':25, 'y':26, 'codepoint':'"' },
	{ 'x':125, 'y':39, 'codepoint':'#' },
	{ 'x':125, 'y':39, 'codepoint':'$' },
	{ 'x':125, 'y':39, 'codepoint':'%' },
	{ 'x':125, 'y':39, 'codepoint':'&' },
	{ 'x':20, 'y':26, 'codepoint':'\'' },
	{ 'x':30, 'y':26, 'codepoint':'(' },
	{ 'x':35, 'y':26, 'codepoint':')' },
	{ 'x':75, 'y':26, 'codepoint':'*' },
	{ 'x':40, 'y':26, 'codepoint':'+' },
	{ 'x':10, 'y':26, 'codepoint':',' },
	{ 'x':45, 'y':26, 'codepoint':'-' },
	{ 'x':0, 'y':26, 'codepoint':'.' },
	{ 'x':70, 'y':26, 'codepoint':'/' },
	{ 'x':0, 'y':39, 'codepoint':'0' },
	{ 'x':5, 'y':39, 'codepoint':'1' },
	{ 'x':10, 'y':39, 'codepoint':'2' },
	{ 'x':15, 'y':39, 'codepoint':'3' },
	{ 'x':20, 'y':39, 'codepoint':'4' },
	{ 'x':25, 'y':39, 'codepoint':'5' },
	{ 'x':30, 'y':39, 'codepoint':'6' },
	{ 'x':35, 'y':39, 'codepoint':'7' },
	{ 'x':40, 'y':39, 'codepoint':'8' },
	{ 'x':45, 'y':39, 'codepoint':'9' },
	{ 'x':25, 'y':26, 'codepoint':':' },
	{ 'x':5, 'y':26, 'codepoint':';' },
	{ 'x':60, 'y':26, 'codepoint':'<' },
	{ 'x':50, 'y':26, 'codepoint':'=' },
	{ 'x':65, 'y':26, 'codepoint':'>' },
	{ 'x':95, 'y':26, 'codepoint':'?' },
	{ 'x':125, 'y':39, 'codepoint':'@' },
	{ 'x':0, 'y':13, 'codepoint':'A' },
	{ 'x':5, 'y':13, 'codepoint':'B' },
	{ 'x':10, 'y':13, 'codepoint':'C' },
	{ 'x':15, 'y':13, 'codepoint':'D' },
	{ 'x':20, 'y':13, 'codepoint':'E' },
	{ 'x':25, 'y':13, 'codepoint':'F' },
	{ 'x':30, 'y':13, 'codepoint':'G' },
	{ 'x':35, 'y':13, 'codepoint':'H' },
	{ 'x':40, 'y':13, 'codepoint':'I' },
	{ 'x':45, 'y':13, 'codepoint':'J' },
	{ 'x':50, 'y':13, 'codepoint':'K' },
	{ 'x':55, 'y':13, 'codepoint':'L' },
	{ 'x':60, 'y':13, 'codepoint':'M' },
	{ 'x':65, 'y':13, 'codepoint':'N' },
	{ 'x':70, 'y':13, 'codepoint':'O' },
	{ 'x':75, 'y':13, 'codepoint':'P' },
	{ 'x':80, 'y':13, 'codepoint':'Q' },
	{ 'x':85, 'y':13, 'codepoint':'R' },
	{ 'x':90, 'y':13, 'codepoint':'S' },
	{ 'x':95, 'y':13, 'codepoint':'T' },
	{ 'x':100, 'y':13, 'codepoint':'U' },
	{ 'x':105, 'y':13, 'codepoint':'V' },
	{ 'x':110, 'y':13, 'codepoint':'W' },
	{ 'x':115, 'y':13, 'codepoint':'X' },
	{ 'x':120, 'y':13, 'codepoint':'Y' },
	{ 'x':125, 'y':13, 'codepoint':'Z' },
	{ 'x':80, 'y':26, 'codepoint':'[' },
	{ 'x':125, 'y':39, 'codepoint':'\\' },
	{ 'x':85, 'y':26, 'codepoint':']' },
	{ 'x':125, 'y':39, 'codepoint':'^' },
	{ 'x':55, 'y':26, 'codepoint':'_' },
	{ 'x':125, 'y':39, 'codepoint':'`' },
	{ 'x':0, 'y':0, 'codepoint':'a' },
	{ 'x':5, 'y':0, 'codepoint':'b' },
	{ 'x':10, 'y':0, 'codepoint':'c' },
	{ 'x':15, 'y':0, 'codepoint':'d' },
	{ 'x':20, 'y':0, 'codepoint':'e' },
	{ 'x':25, 'y':0, 'codepoint':'f' },
	{ 'x':30, 'y':0, 'codepoint':'g' },
	{ 'x':35, 'y':0, 'codepoint':'h' },
	{ 'x':40, 'y':0, 'codepoint':'i' },
	{ 'x':45, 'y':0, 'codepoint':'j' },
	{ 'x':50, 'y':0, 'codepoint':'k' },
	{ 'x':55, 'y':0, 'codepoint':'l' },
	{ 'x':60, 'y':0, 'codepoint':'m' },
	{ 'x':65, 'y':0, 'codepoint':'n' },
	{ 'x':70, 'y':0, 'codepoint':'o' },
	{ 'x':75, 'y':0, 'codepoint':'p' },
	{ 'x':80, 'y':0, 'codepoint':'q' },
	{ 'x':85, 'y':0, 'codepoint':'r' },
	{ 'x':90, 'y':0, 'codepoint':'s' },
	{ 'x':95, 'y':0, 'codepoint':'t' },
	{ 'x':100, 'y':0, 'codepoint':'u' },
	{ 'x':105, 'y':0, 'codepoint':'v' },
	{ 'x':110, 'y':0, 'codepoint':'w' },
	{ 'x':115, 'y':0, 'codepoint':'x' },
	{ 'x':120, 'y':0, 'codepoint':'y' },
	{ 'x':125, 'y':0, 'codepoint':'z' },
	{ 'x':100, 'y':26, 'codepoint':'{' },
	{ 'x':125, 'y':39, 'codepoint':'|' },
	{ 'x':105, 'y':26, 'codepoint':'}' },
	{ 'x':125, 'y':39, 'codepoint':'~' },
]

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
		#print y, y - 4, y - 8
		
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
		
		for x in xrange(0, 4):
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
			
			encoded_bit -= 2
			pixel_index += 4
	
	return ['0x{0:08X}'.format(encoded_upper), '0x{0:08X}'.format(encoded_middle), '0x{0:08X}'.format(encoded_lower)]

output_file = open('systemfont.txt', 'w')
output_file.write('// encoded using systembuilder.py\n')
output_file.write('// for reference, please see systemfont.tga\n')
output_file.write('\n')
output_file.write('static const unsigned int s_GlyphBitmapsEncoded[] = {\n')

for g in glyphs:
	encoded = EncodeGlyph(g['x'], g['y'])
	output_file.write('\t{0}, {1}, {2}, // {3}\n'.format(encoded[0], encoded[1], encoded[2], g['codepoint']))

output_file.write('};')