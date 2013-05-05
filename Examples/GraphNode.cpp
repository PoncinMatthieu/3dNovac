
#include <Nc/Core/Utils/Logger.h>
#include <Nc/Core/Graph/DotGraph.h>
#include <Nc/Core/Graph/NNodePolitic.h>

using namespace Nc;
using namespace Nc::Graph;

int main()
{
    Node<int>   node(1);

    node.AddChild(new Node<int>(10));
    node.AddChild(new Node<int>(10));
    node.AddChild(new Node<int>(10));
    node.AddChild(new Node<int>(0));
    node.AddChild(new Node<int>(120));
    node.AddChild(new Node<int>(115));
    node.AddChild(new Node<int>(1430));

    node.Child(2)->AddChild(new Node<int>(345));

    LOG << "Printing tree... ";
    DotGraph<Node<int, ListNodePolitic> >::Save("testListNode.dot", node);
    LOG << "done" << std::endl;
    LOG << "Run the following command to see the result as an image:" << std::endl;
    LOG << "\tdot testListNode.dot -Tpng > result.png" << std::endl;
    return 0;
}
