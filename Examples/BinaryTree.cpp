/** \file BinaryTree.cpp
	\todo The BinaryTree Example doesn't seem to work with VisualStudio, the templates failed to compile.
*/

#include <Nc/Core/Graph/DotGraph.h>
#include <Nc/Core/Graph/BinaryTree.h>
#include <Nc/Core/Utils/Logger.h>

using namespace Nc;
using namespace Nc::Graph;

int main()
{
  BinaryTree<int> t;

  t.Insert(100);
  t.Insert(1);
  t.Insert(24);
  t.Insert(3);
  t.Insert(23);
  t.Insert(5);
  t.Insert(7);
  t.Insert(34);
  t.Insert(8);
  t.Insert(3);

  for (BinaryTree<int>::NodeType *n = t.First(); n != NULL; n = n->Next())
    LOG << n->data << " ";
  LOG << std::endl;
  for (BinaryTree<int>::NodeType *n = t.Last(); n != NULL; n = n->Prev())
    LOG << n->data << " ";
  LOG << std::endl;

  LOG << "print" << std::endl;
  if (t.Root() != NULL)
    DotGraph<BinaryTree<int>::NodeType >::Save("testBinaryNode.dot", *t.Root());
  return 0;
}
