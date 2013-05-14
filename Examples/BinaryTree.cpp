
#include <Nc/Core/Graph/DotGraph.h>
#include <Nc/Core/Graph/BinaryTree.h>
#include <Nc/Core/Utils/Logger.h>
#include <Nc/Core/Math/Math.h>

using namespace Nc;
using namespace Nc::Graph;

int main()
{
  BinaryTree<int> t;

  // insert 100 random values inside the tree
  Nc::Math::InitRand();
  for (unsigned int i = 0; i < 100; ++i)
    t.Insert(Nc::Math::Rand(0, 1000));

  LOG << "A BinaryTree can be use to sort numbers at insertion." << std::endl;
  LOG << "Printing sorted numbers:" << std::endl;
  for (BinaryTree<int>::NodeType *n = t.First(); n != NULL; n = n->Next())
    LOG << n->data << " ";
  LOG << std::endl;

  if (t.Root() != NULL)
    {
      LOG << "Printing tree... ";
      DotGraph<BinaryTree<int>::NodeType >::Save("testBinaryNode.dot", *t.Root());
      LOG << "done" << std::endl;
      LOG << "Run the following command to see the result as an image:" << std::endl;
      LOG << "\tdot testBinaryNode.dot -Tpng > result.png" << std::endl;
    }
  return 0;
}
