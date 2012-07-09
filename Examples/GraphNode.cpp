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
    node.AddChild(new Node<int>(120));
    node.AddChild(new Node<int>(1430));

    node.Child(2)->AddChild(new Node<int>(345));

    LOG << "print" << std::endl;
    DotGraph<Node<int, ListNodePolitic> >::Save("testListNode.dot", node);
    return 0;
}
