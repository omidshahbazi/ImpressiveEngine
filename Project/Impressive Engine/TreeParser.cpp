/*/////////////////////////////////////////////////////////////////////////////////
///
///  Impressive Engine
///
/// Copyright (c) 2012-2013 Impressive Reality team
//
/// The license is
///
/// Permission is denied, to any person or company
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// Project leader : O.Shahbazi <sh_omid_m@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////*/
#include "TreeParser.h"


BEGIN_NAMESPACE
TreeParser::TreeParser(const std::string &FilePath, const std::string &StartPath)
{
	m_Root = new TreeElement;
	m_Root->Name = "RootElement";

	if (FilePath.size())
	{
		ptree tree;
		read_xml(FilePath, tree);
	
		BOOST_FOREACH(const ptree::value_type &v, tree.get_child(StartPath))
		{
			CreateElement(m_Root, v);
		}
	}

	//ptree::const_iterator end = m_Properties.end();
	//for (ptree::const_iterator it = m_Properties.begin(); it != end; it++)
	//{
	//	ptree::const_iterator endv = it->second.get_child("").end();
	//	for (ptree::const_iterator itv = it->second.get_child("").begin(); itv != endv; itv++)
	//	{
	//		std::cout << itv->first.data() << "---------------------------\n";
	//		BOOST_FOREACH(const ptree::value_type &a, itv->second.get_child(""))
	//			std::cout << a.first.data() << "\n";
	//	}
	//}
	
	//if (v.first == "ATTRIBUTES")
	//{
	//	BOOST_FOREACH(const ptree::value_type &a, m_Properties.get_child("SCENE.ATTRIBUTES"))
	//	if (a.first == "ATTRIBUTES")
	//	{

	//	}
	//}

	
	//<debug>
	//    <filename>debug.log</filename>
	//    <modules>
	//        <module>Finance</module>
	//        <module>Admin</module>
	//        <module>HR</module>
	//    </modules>
	//    <level>2</level>
	//</debug>

	// Saves the debug_settings structure to the specified XML file
	//void save(const std::string &filename)
	//{
	//	// Create an empty property tree object
	//	using boost::property_tree::ptree;
	//	ptree pt;
	//
	//	// Put log filename in property tree
	//	pt.put("debug.filename", m_file);
	//
	//	// Put debug level in property tree
	//	pt.put("debug.level", m_level);
	//
	//	// Iterate over the modules in the set and put them in the
	//	// property tree. Note that the put function places the new
	//	// key at the end of the list of keys. This is fine most of
	//	// the time. If you want to place an item at some other place
	//	// (i.e. at the front or somewhere in the middle), this can
	//	// be achieved using a combination of the insert and put_own
	//	// functions.
	//	BOOST_FOREACH(const std::string &name, m_modules)
	//		pt.put("debug.modules.module", name, true);
	//
	//	// Write the property tree to the XML file.
	//	write_xml(filename, pt);
	//}
}


TreeParser::~TreeParser(void)
{
	delete m_Root;
}


void TreeParser::WriteToFile(const std::string &FilePath)
{
	ptree tree;

	BOOST_FOREACH(TreeElement *elem, m_Root->Children)
		CreateValue(tree, elem);

	write_xml(FilePath, tree);
}

	
TreeElement *TreeParser::CreateElement(TreeElement *Parent, ptree::value_type Value)
{
	TreeElement *elem = new TreeElement(Parent);

	elem->Name = Value.first;

	bool hasChildren = false;

	BOOST_FOREACH(const ptree::value_type &v, Value.second.get_child(""))
	{
		if (v.first == "<xmlattr>")
		{
			ptree::const_iterator end = Value.second.get_child("<xmlattr>").end();
			for (ptree::const_iterator it = Value.second.get_child("<xmlattr>").begin(); it != end; it++)
				elem->Attributes.push_back(TreeElementAttribute(it->first, it->second.data()));
		}
		else
			CreateElement(elem, v);

		hasChildren = true;
	}

	if (!hasChildren)
		elem->Value = Value.second.data();

	return elem;
}


void TreeParser::CreateValue(ptree &tree, TreeElement *Current)
{
	ptree &curr = tree.add(Current->Name, "");

	if (Current->Children.size())
	{
		BOOST_FOREACH(TreeElement *elem, Current->Children)
			CreateValue(curr, elem);
	}
	else
		curr.put_value(Current->Value);

	BOOST_FOREACH(TreeElementAttribute attr, Current->Attributes)
		curr.add("<xmlattr>." + attr.first, attr.second);
}


//BEGIN_DEFINE_SCRIPT_REGISTRATION(TreeParser)
//END_DEFINE_SCRIPT_REGISTRATION()

END_NAMESPACE