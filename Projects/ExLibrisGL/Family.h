#ifndef _FAMILY_H_
#define _FAMILY_H_

namespace ExLibris
{
	class Library;
};

namespace ExLibris
{

	class Family
	{
	
	public:
	
		Family(Library* a_Library, const std::string& a_Name);
		~Family();
	
		Library* GetLibrary() const;

		const std::string& GetName() const;

	private:

		Library* m_Library;
		std::string m_Name;
	
	}; // class Family

}; // namespace ExLibris

#endif