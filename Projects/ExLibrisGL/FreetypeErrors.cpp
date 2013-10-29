#include "ExLibrisGL.PCH.h"

#include "FreetypeErrors.h"

#include <sstream>

namespace ExLibris {
namespace Freetype {

#define CASE_LABEL2(prefix, name) FT_ ##prefix ##name
#define CASE_LABEL(_prefix, _name) CASE_LABEL2(##_prefix, ##_name)
#define TRANSLATE_CASE(_name, _translation) \
	case CASE_LABEL(Err_, ##_name): \
		{ \
			static const std::string CASE_LABEL(translated_, ##_name) = _translation; \
			return CASE_LABEL(translated_, ##_name); \
		} break;

	std::string TranslateError(FT_Error a_Error)
	{
		switch (a_Error)
		{

		// general

		TRANSLATE_CASE(Ok, "no errors")
		TRANSLATE_CASE(Cannot_Open_Resource, "cannot open resource")		TRANSLATE_CASE(Unknown_File_Format, "unknown file format")		TRANSLATE_CASE(Invalid_File_Format, "invalid file format")		TRANSLATE_CASE(Invalid_Version, "invalid FreeType version")		TRANSLATE_CASE(Lower_Module_Version, "module version is too low")		TRANSLATE_CASE(Invalid_Argument, "invalid argument")		TRANSLATE_CASE(Unimplemented_Feature, "feature not implemented")		TRANSLATE_CASE(Invalid_Table, "invalid table")		TRANSLATE_CASE(Invalid_Offset, "invalid offset in table")		TRANSLATE_CASE(Array_Too_Large, "could not allocate array: out of memory")		TRANSLATE_CASE(Missing_Module, "missing module")		TRANSLATE_CASE(Missing_Property, "missing property")		TRANSLATE_CASE(Invalid_Glyph_Index, "invalid glyph index")		TRANSLATE_CASE(Invalid_Character_Code, "invalid character code")		TRANSLATE_CASE(Cannot_Render_Glyph, "unsupported glyph image format")		TRANSLATE_CASE(Invalid_Outline, "invalid outline for glyph")		TRANSLATE_CASE(Invalid_Composite, "invalid composite glyph")		TRANSLATE_CASE(Too_Many_Hints, "too many hints for glyph")		TRANSLATE_CASE(Invalid_Pixel_Size, "invalid pixel size")		TRANSLATE_CASE(Invalid_Handle, "invalid object handle")		TRANSLATE_CASE(Invalid_Library_Handle, "invalid library handle")		TRANSLATE_CASE(Invalid_Driver_Handle, "invalid module handle")		TRANSLATE_CASE(Invalid_Face_Handle, "invalid face handle")		TRANSLATE_CASE(Invalid_Size_Handle, "invalid size handle")		TRANSLATE_CASE(Invalid_Slot_Handle, "invalid glyph slot handle")		TRANSLATE_CASE(Invalid_CharMap_Handle, "invalid charmap handle")		TRANSLATE_CASE(Invalid_Cache_Handle, "invalid cache manager handle")		TRANSLATE_CASE(Invalid_Stream_Handle, "invalid stream handle")		TRANSLATE_CASE(Too_Many_Drivers, "too many modules")		TRANSLATE_CASE(Too_Many_Extensions, "too many extensions")		TRANSLATE_CASE(Out_Of_Memory, "out of memory")		TRANSLATE_CASE(Unlisted_Object, "unlisted object")
		TRANSLATE_CASE(Cannot_Open_Stream, "cannot open stream")
		TRANSLATE_CASE(Invalid_Stream_Seek, "invalid stream seek")
		TRANSLATE_CASE(Invalid_Stream_Skip, "invalid stream skip")
		TRANSLATE_CASE(Invalid_Stream_Read, "invalid stream read")
		TRANSLATE_CASE(Invalid_Stream_Operation, "invalid stream operation")
		TRANSLATE_CASE(Invalid_Frame_Operation, "invalid frame operation")
		TRANSLATE_CASE(Nested_Frame_Access, "nested frame access")
		TRANSLATE_CASE(Invalid_Frame_Read, "invalid frame read")
		TRANSLATE_CASE(Raster_Uninitialized, "raster uninitialized")
		TRANSLATE_CASE(Raster_Corrupted, "raster corrupted")
		TRANSLATE_CASE(Raster_Overflow, "raster overflow")
		TRANSLATE_CASE(Raster_Negative_Height, "negative height while rastering")
		TRANSLATE_CASE(Too_Many_Caches, "too many registered caches")
		TRANSLATE_CASE(Invalid_Opcode, "invalid opcode")
		TRANSLATE_CASE(Too_Few_Arguments, "too few arguments")
		TRANSLATE_CASE(Stack_Overflow, "stack overflow")
		TRANSLATE_CASE(Code_Overflow, "code overflow")
		TRANSLATE_CASE(Bad_Argument, "bad argument")
		TRANSLATE_CASE(Divide_By_Zero, "division by zero")
		TRANSLATE_CASE(Invalid_Reference, "invalid reference")
		TRANSLATE_CASE(Debug_OpCode, "found debug opcode")
		TRANSLATE_CASE(ENDF_In_Exec_Stream, "found ENDF opcode in execution stream")
		TRANSLATE_CASE(Nested_DEFS, "nested DEFS")
		TRANSLATE_CASE(Invalid_CodeRange, "invalid code range")
		TRANSLATE_CASE(Execution_Too_Long, "execution context too long")
		TRANSLATE_CASE(Too_Many_Function_Defs, "too many function definitions")
		TRANSLATE_CASE(Too_Many_Instruction_Defs, "too many instruction definitions")
		TRANSLATE_CASE(Table_Missing, "SFNT font table missing")
		TRANSLATE_CASE(Horiz_Header_Missing, "horizontal header (hhea) table missing")
		TRANSLATE_CASE(Locations_Missing, "locations (loca) table missing")
		TRANSLATE_CASE(Name_Table_Missing, "name table missing")
		TRANSLATE_CASE(CMap_Table_Missing, "character map (cmap) table missing")
		TRANSLATE_CASE(Hmtx_Table_Missing, "horizontal metrics (hmtx) table missing")
		TRANSLATE_CASE(Post_Table_Missing, "PostScript (post) table missing")
		TRANSLATE_CASE(Invalid_Horiz_Metrics, "invalid horizontal metrics")
		TRANSLATE_CASE(Invalid_CharMap_Format, "invalid character map (cmap) format")
		TRANSLATE_CASE(Invalid_PPem, "invalid ppem value")
		TRANSLATE_CASE(Invalid_Vert_Metrics, "invalid vertical metrics")
		TRANSLATE_CASE(Could_Not_Find_Context, "could not find context")
		TRANSLATE_CASE(Invalid_Post_Table_Format, "invalid PostScript (post) table format")
		TRANSLATE_CASE(Invalid_Post_Table, "invalid PostScript (post) table")
		TRANSLATE_CASE(Syntax_Error, "opcode syntax error")
		TRANSLATE_CASE(Stack_Underflow, "argument stack underflow")
		TRANSLATE_CASE(Ignore, "ignore")
		TRANSLATE_CASE(No_Unicode_Glyph_Name, "no Unicode glyph name found")
		TRANSLATE_CASE(Glyph_Too_Big, "glyph to big for hinting")
		TRANSLATE_CASE(Missing_Startfont_Field, "`STARTFONT' field missing")
		TRANSLATE_CASE(Missing_Font_Field, "`FONT' field missing")
		TRANSLATE_CASE(Missing_Size_Field, "`SIZE' field missing")
		TRANSLATE_CASE(Missing_Fontboundingbox_Field, "`FONTBOUNDINGBOX' field missing")
		TRANSLATE_CASE(Missing_Chars_Field, "`CHARS' field missing")
		TRANSLATE_CASE(Missing_Startchar_Field, "`STARTCHAR' field missing")
		TRANSLATE_CASE(Missing_Encoding_Field, "`ENCODING' field missing")
		TRANSLATE_CASE(Missing_Bbx_Field, "`BBX' field missing")
		TRANSLATE_CASE(Bbx_Too_Big, "`BBX' too big")
		TRANSLATE_CASE(Corrupted_Font_Header, "Font header corrupted or missing fields")
		TRANSLATE_CASE(Corrupted_Font_Glyphs, "Font glyphs corrupted or missing fields")		default:			{				std::stringstream ss;				ss << "Unknown error: (" << a_Error << ")";				return ss.str();			} break;		}

		return "";
	}

#undef CASE_LABEL2
#undef CASE_LABEL
#undef TRANSLATE_CASE

}; // namespace Freetype
}; // namespace ExLibris