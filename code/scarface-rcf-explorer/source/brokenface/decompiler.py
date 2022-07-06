#! /usr/bin/env python3

import argparse
import logging

from pathlib import Path
from os import remove
from sys import stdout
from struct import pack, unpack
from os.path import getsize

def getArgs():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "fnames",
        metavar="FILE_NAME",
        type=str,
        nargs="+",
        help="name of the file to be decompiled"
    )

    parser.add_argument(
        "--parse-only",
        dest="parseOnly",
        action="store_const",
        const="parseOnly",
        default=False,
        help="only parse the file and dump the structures"
    )

    parser.add_argument(
        "--debug",
        dest="debug",
        action="store_const",
        const="debug",
        default=False,
        help="set logging level to DEBUG"
    )

    opt = parser.parse_args()

    return opt.fnames, opt.parseOnly, opt.debug

fnames, parseOnly, debug = getArgs()

'''
Template for TorqueScript operation
'''
class Operation:
    # By default, not included in any category:
    isArithmetic = False
    isBoolean = False
    isBitwise = False
    isString = False
    isAccess = False

    '''
    Constructs an Operation object
    @param  operands    List of operands
    '''
    def __init__(self, operands):
        self.operands = operands

    '''
    Equal operator override
    @params obj     Object to compare this instance with
    '''
    def __eq__(self, obj):
        # Two statements are equal if their string representations are equal:
        return str(self) == str(obj)


'''
TorqueScript Add operation
'''
class Add(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        return " + ".join(str(op) for op in self.operands)


'''
TorqueScript Subtraction operation
'''
class Sub(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        return " - ".join(str(op) for op in self.operands)


'''
TorqueScript Multiply operation
'''
class Mul(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        return " * ".join(str(op) for op in self.operands)


'''
TorqueScript Division operation
'''
class Div(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        return " / ".join(str(op) for op in self.operands)


'''
TorqueScript Modulo operation
'''
class Mod(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        return " % ".join(str(op) for op in self.operands)


'''
TorqueScript Modulo operation
'''
class Neg(Operation):
    isArithmetic = True

    '''
    String representation override
    '''
    def __str__(self):
        if isinstance(self.operands[0], float):
            return "-" + str(self.operands[0])
        else:
            return "-1.0 * " + str(self.operands[0])


'''
TorqueScript Negation operation
'''
class Not(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        # If negates a boolean equal operation:
        if isinstance(self.operands[0], StringEqual):
            operands = self.operands[0].operands
            operation = NotEqual(operands)
            return str(operation)
        # If negates a boolean not equal operation:
        elif isinstance(self.operands[0], StringNotEqual):
            operands = self.operands[0].operands
            operation = Equal(operands)
            return str(operation)
        # If negates a less than operation:
        elif isinstance(self.operands[0], Less):
            operands = self.operands[0].operands
            operation = GreaterOrEqual(operands)
            return str(operation)
        # If negates a less than or equal to operation:
        elif isinstance(self.operands[0], LessOrEqual):
            operands = self.operands[0].operands
            operation = Greater(operands)
            return str(operation)
        # If negates a greater than operation:
        elif isinstance(self.operands[0], Greater):
            operands = self.operands[0].operands
            operation = LessOrEqual(operands)
            return str(operation)
        # If negates a greater than or equal to operation:
        elif isinstance(self.operands[0], GreaterOrEqual):
            operands = self.operands[0].operands
            operation = Less(operands)
            return str(operation)
        # If negates a string equal operation:
        elif isinstance(self.operands[0], StringEqual):
            operands = self.operands[0].operands
            operation = StringNotEqual(operands)
            return str(operation)
        # If negates a string not equal operation:
        elif isinstance(self.operands[0], StringNotEqual):
            operands = self.operands[0].operands
            operation = StringEqual(operands)
            return str(operation)
        else:
            return "!(" + str(self.operands[0]) + ")"


'''
TorqueScript Equal operation
'''
class Equal(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " == ".join(str(op) for op in self.operands)


'''
TorqueScript Not Equal operation
'''
class NotEqual(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " != ".join(str(op) for op in self.operands)


'''
TorqueScript Less Than operation
'''
class Less(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " < ".join(str(op) for op in self.operands)


'''
TorqueScript Less Than Or Equal To operation
'''
class LessOrEqual(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " <= ".join(str(op) for op in self.operands)


'''
TorqueScript Greater Than operation
'''
class Greater(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " > ".join(str(op) for op in self.operands)


'''
TorqueScript Greater Than Or Equal To operation
'''
class GreaterOrEqual(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " >= ".join(str(op) for op in self.operands)


'''
TorqueScript And operation
'''
class And(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " && ".join(str(op) for op in self.operands)


'''
TorqueScript Or operation
'''
class Or(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " || ".join(str(op) for op in self.operands)


'''
TorqueScript One's Complement operation
'''
class Complement(Operation):
    isBoolean = True

    '''
    String representation override
    '''
    def __str__(self):
        return " ~ ".join(str(op) for op in self.operands)


'''
TorqueScript Bitwise And operation
'''
class BitAnd(Operation):
    isBitwise = True

    '''
    String representation override
    '''
    def __str__(self):
        return " & ".join(str(op) for op in self.operands)


'''
TorqueScript Bitwise Or operation
'''
class BitOr(Operation):
    isBitwise = True

    '''
    String representation override
    '''
    def __str__(self):
        return " | ".join(str(op) for op in self.operands)


'''
TorqueScript Bitwise Xor operation
'''
class Xor(Operation):
    isBitwise = True

    '''
    String representation override
    '''
    def __str__(self):
        return " ^ ".join(str(op) for op in self.operands)


'''
TorqueScript Shift Left operation
'''
class ShiftLeft(Operation):
    isBitwise = True

    '''
    String representation override
    '''
    def __str__(self):
        return " << ".join(str(op) for op in self.operands)


'''
TorqueScript Shift Right operation
'''
class ShiftRight(Operation):
    isBitwise = True

    '''
    String representation override
    '''
    def __str__(self):
        return " >> ".join(str(op) for op in self.operands)


'''
TorqueScript String Equal operation
'''
class StringEqual(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " $= ".join(str(op) for op in self.operands)


'''
TorqueScript String Not Equal operation
'''
class StringNotEqual(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " !$= ".join(str(op) for op in self.operands)


'''
TorqueScript String Concatenation operation
'''
class Concat(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " @ ".join(str(op) for op in self.operands)


'''
TorqueScript String Newline Concatenation operation
'''
class ConcatNl(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " NL ".join(str(op) for op in self.operands)


'''
TorqueScript String Tab Concatenation operation
'''
class ConcatTab(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " TAB ".join(str(op) for op in self.operands)


'''
TorqueScript String Space Concatenation operation
'''
class ConcatSpc(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return " SPC ".join(str(op) for op in self.operands)


'''
TorqueScript String Comma Concatenation operation (for array access)
'''
class ConcatComma(Operation):
    isString = True

    '''
    String representation override
    '''
    def __str__(self):
        return ", ".join(str(op) for op in self.operands)


'''
TorqueScript Array Access operation
'''
class ArrayAccess(Operation):
    isAccess = True

    '''
    String representation override
    '''
    def __str__(self):
        return str(self.operands[0]) + "[" + str(self.operands[1]) + "]"


'''
TorqueScript Field Access operation
'''
class FieldAccess(Operation):
    isAccess = True

    '''
    String representation override
    '''
    def __str__(self):
        return ".".join(str(op) for op in self.operands)


'''
Template class for a node of the tree
'''
class Node:
    '''
    Constructs a Node object
    '''
    def __init__(self):
        self.parent = None
        self.children = []
        self.block = 0

    '''
    Equal operator override
    @params obj     Object to compare this instance with
    '''
    def __eq__(self, obj):
        # Two statements are equal if their string representations are equal:
        return str(self) == str(obj)

    '''
    Appends the given node to the children list
    @params child   Node to be appended as child
    '''
    def append(self, child):
        child.parent = self
        self.children.append(child)


'''
TorqueScript assignment
'''
class Assignment(Node):
    '''
    Constructs an Assignment object
    @param  left    Left operand of assignment
    @param  right   Right operand of assignment
    '''
    def __init__(self, left, right):
        # Inherit characteristics from Node:
        super().__init__()

        self.left = left
        self.right = right

        if isinstance(self.right, ObjCreation):
            # Move children of object to assignment
            self.children = self.right.children
            # Indicate if there is a block of code (children):
            self.block = self.right.block
        else:
            # No block otherwise:
            self.block = False

    '''
    String representation override
    '''
    def __str__(self):
        return str(self.left) + " = " + str(self.right)


'''
TorqueScript break from loop statement
'''
class Break(Node):
    '''
    Constructs an Assignment object
    '''
    def __init__(self):
        # Inherit characteristics from Node:
        super().__init__()

        # No block declaration:
        self.block = False

    '''
    String representation override
    '''
    def __str__(self):
        return "break"


'''
TorqueScript else statement
'''
class Else(Node):
    '''
    Constructs an Assignment object
    '''
    def __init__(self):
        # Inherit characteristics from Node:
        super().__init__()

        # Block declaration:
        self.block = True

    '''
    String representation override
    '''
    def __str__(self):
        return "else"


'''
File information header comment (also root of tree)
'''
class torqueFile(Node):
    '''
    Constructs a File object
    @param  name    Name of the file being decompiled
    '''
    def __init__(self, name):
        # Inherit characteristics from Node:
        super().__init__()

        self.name = name

        # No block declaration:
        self.block = False

    '''
    String representation override
    '''
    def __str__(self):
        return "// Decompiled file: " + self.name


'''
TorqueScript function call
'''
class FuncCall(Node):
    callTypes = {
        0:  "Function",
        1:  "Method",
        2:  "Parent"
    }

    '''
    Constructs a FuncCall object
    @param  name        Name of the function being called
    @param  namespace   Namespace of function being called
    @param  callType    Type of call (function, method or parent)
    @param  argv        List of arguments passed to function
    '''
    def __init__(self, name, namespace, callType, argv):
        # Inherit characteristics from Node:
        super().__init__()

        self.name = name
        self.namespace = namespace
        self.callType = callType

        if self.callTypes[self.callType] == "Method":
            # First argument is actually the name of the object whose method is being called:
            self.objName = argv[0]
            self.argv = argv[1:]
        else:
            self.argv = argv

        # No block declaration:
        self.block = False

    '''
    String representation override
    '''
    def __str__(self):
        if self.namespace == "":
            if self.callTypes[self.callType] == "Parent":
                baseStr = "base::" # TODO: Is this the right syntax?
            else:
                baseStr = ""
        else:
            baseStr = self.namespace + "::"

        if self.callTypes[self.callType] == "Method":
            baseStr += str(self.objName) + "."

        return baseStr + self.name + "(" + str(ConcatComma(self.argv)) + ")"


'''
TorqueScript function declaration
'''
class FuncDecl(Node):
    '''
    Constructs a FuncDecl object
    @param  name        Name of the function being declared
    @param  namespace   Namespace of function being declared
    @param  package     Package of function being declared
    @param  hasBody     Boolean value indicating if function has body (block of code)
    @param  end         Code index of end of declaration
    @param  argc        Number of arguments passed to function
    @param  argv        List of arguments passed to function
    '''
    def __init__(self, name, namespace, package, hasBody, end, argc, argv):
        # Inherit characteristics from Node:
        super().__init__()

        self.name = name
        self.namespace = namespace
        self.package = package # TODO: Unused
        self.hasBody = hasBody # TODO: Unused
        self.end = end
        self.argc = argc
        self.argv = argv

        # Block declaration:
        self.block = True

    '''
    String representation override
    '''
    def __str__(self):
        if self.namespace == "":
            return "function " + self.name + "(" + str(ConcatComma(self.argv)) + ")"
        else:
            return "function " + self.namespace + "::" + self.name + "(" + str(ConcatComma(self.argv)) + ")"


'''
TorqueScript if statement
'''
class If(Node):
    '''
    Constructs an If object
    @param  condition   Condition of if statement
    '''
    def __init__(self, condition):
        # Inherit characteristics from Node:
        super().__init__()

        self.condition = condition

        # Handle to else statement, if any:
        self.elseHandle = None

        # Block declaration:
        self.block = 1

    '''
    String representation override
    '''
    def __str__(self):
        return "if (" + str(self.condition) + ")"
    

'''
TorqueScript object creation
'''
class ObjCreation(Node):
    '''
    Constructs ObjCreation object
    @param  parentName  Name of parent object
    @param  mistery     Some value I do not know what is about
    @param  argv        List of arguments passed to object
    '''
    def __init__(self, parentName, mistery, argv):
        # Inherit characteristics from Node:
        super().__init__()

        self.parentName = parentName
        self.mistery = mistery # TODO: Find ou what this is about

        # First argument is object type:
        self.objType = argv[0]
        self.argv = argv[1:]

        if self.argv:
            # Second argument is object name:
            self.argv[0] = "Name : " + str(self.argv[0])

        # Block declaration:
        self.block = True

    '''
    String representation override
    '''
    def __str__(self):
        return "new " + self.objType + "(" + str(ConcatComma(self.argv)) + ")"


'''
TorqueScript return statement
'''
class Return(Node):
    '''
    Constructs a Return object
    @param  value   Return value
    '''
    def __init__(self, value):
        # Inherit characteristics from Node:
        super().__init__()

        self.value = value

        # No block declaration:
        self.block = False

    '''
    String representation override
    '''
    def __str__(self):
        if self.value == None:
            return "return"
        else:
            return "return " + str(self.value)


'''
TorqueScript while statement
'''
class While(Node):
    '''
    Constructs an While object
    @param  condition   Condition of while statement
    '''
    def __init__(self, condition):
        # Inherit characteristics from Node:
        super().__init__()

        self.condition = condition

        # Block declaration:
        self.block = True

    '''
    String representation override
    '''
    def __str__(self):
        return "while (" + str(self.condition) + ")"


'''
Tree of TorqueScript statements (Nodes)
'''
class Tree:
    '''
    Constructs a Tree object
    @param  root    Root node of tree
    '''
    def __init__(self, root):
        self.root = root

        # Pointer to node being currently manipulated:
        self.curNode = root
        # Characters to be prepended to each line of code for indentation:
        self.indent = ""

    '''
    Appends a node to the current node of the tree
    @param  node    Node to be appended
    '''
    def append(self, node):
        self.curNode.append(node)

    '''
    Replaces the current node by the given one
    @param  new     Node to replace the current one by
    '''
    def replace(self, new):
        old = self.curNode

        parent = old.parent
        children = old.children

        # Replace reference on parent's children list:
        if parent is not None:
            parent.children[parent.children.index(old)] = new

        # Replace references on children's parent field:
        for child in children:
            child.parent = new

        new.parent = parent
        new.children = children

        self.curNode = new

    '''
    Rewinds tree (resets current node to root)
    '''
    def rewind(self):
        self.curNode = self.root

    '''
    Focuses (change current node pointer to) on child node at given index
    @param  idx     Index of child on children list (default -1 - last appended child)
    '''
    def focusChild(self, idx=-1):
        self.curNode = self.curNode.children[idx]

    '''
    Focuses (change current node pointer to) on parent node
    '''
    def focusParent(self):
        self.curNode = self.curNode.parent

    '''
    Retrieves focused (current) node
    '''
    def getFocused(self):
        return self.curNode

    '''
    Formats tree as text (source code)
    @param  sink    Stream to dump output to (default stdout)
    '''
    def format(self, sink=stdout):
        # Get current node:
        thisNode = self.curNode

        # Print indented line of code:
        print(self.indent + str(thisNode), end="", file=sink)

        # If declares a block:
        if thisNode.block:
            # Print newline:
            print("", file=sink)
            # Open brackets:
            print(self.indent + "{", file=sink)
            # Indent:
            self.indent += "\t" * thisNode.block
        else:
            print(";", file=sink)

        # Call recursively for all children:
        for child in thisNode.children:
            self.curNode = child
            self.format(sink=sink)

        # If declares a block:
        if thisNode.block:
            # Unindent:
            self.indent = self.indent[:-thisNode.block]
            # Close brackets:
            print(self.indent + "}", file=sink)

        # Restore current node:
        self.curNode = thisNode



'''
Generic methods for manipulating byte streams
'''
class binaryReading():
    '''
    Constructs a Reading object
    @param  byteStream  Byte stream to be processed
    @param  endian      Endianess of byte stream
    '''
    def __init__(self, byteStream, endian):
        self.byteStream = byteStream
        #print(self.byteStream)
        self.endian = endian
        #print(self.endian)
        self.pointer = 0
        #print(self.pointer)

    '''
    Reads bytes from byte stream
    @param  n       Number of bytes to be read
    @return bytes   Bytes read
    '''
    def read(self, n):
        rd = self.byteStream[self.pointer:self.pointer+n]

        if len(rd) != n:
            raise IndexError("Index out of range")

        self.pointer += n

        return rd

    '''
    Reads a single byte from byte stream
    @return bytes   Byte read
    '''
    def read8(self):
        return self.read(1)

    '''
    Reads two bytes from byte stream
    @return bytes   Bytes read
    '''
    def read16(self):
        return self.read(2)

    '''
    Reads four bytes from byte stream
    @return bytes   Bytes read
    '''
    def read32(self):
        return self.read(4)

    '''
    Reads eight bytes from byte stream
    @return bytes   Bytes read
    '''
    def read64(self):
        return self.read(8)

    '''
    Reads bytes until a null byte is found and return the resultant string
    @param  max     Maximum amount of bytes to be read
    @return string  String read
    '''
    def readString(self, max):
        byteStream = b''
        idx = 1
        rd = self.read8()
        while idx <= max and rd != b'\x00':
            byteStream += rd
            rd = self.read8()
            idx += 1

        return byteStream

    '''
    Reads a byte and unpacks it into an unsigned integer
    @return int 8-bits long unsigned integer
    '''
    def unpackUint8(self):
        return int.from_bytes(self.read8(), byteorder=self.endian, signed=False)

    '''
    Reads two bytes and unpacks it into an unsigned integer
    @return int 16-bits long unsigned integer
    '''
    def unpackUint16(self, endian=None):
        if endian is None:
            return int.from_bytes(self.read16(), byteorder=self.endian, signed=False)
        else:
            return int.from_bytes(self.read16(), byteorder=endian, signed=False)

    '''
    Reads four bytes and unpacks it into an unsigned integer
    @return int 32-bits long unsigned integer
    '''
    def unpackUint32(self, endian=None):
        if endian is None:
            return int.from_bytes(self.read32(), byteorder=self.endian, signed=False)
        else:
            return int.from_bytes(self.read32(), byteorder=endian, signed=False)

    structEndian = {
        "little":   "<",
        "big":      ">"
    }

    '''
    Reads four bytes and unpacks it into a float
    @return int 32-bits long float
    '''
    def unpackFloat32(self, endian=None):
        if endian is None:
            return unpack(self.structEndian[self.endian] + "f", self.read32())[0]
        else:
            return unpack(self.structEndian[endian] + "f", self.read32())[0]

    '''
    The same as read, but without moving the reading pointer
    @param  n       Number of bytes to be looked up
    @return bytes   Bytes looked up
    '''
    def lookup(self, n):
        rd = self.byteStream[self.pointer:self.pointer+n]

        if len(rd) != n:
            raise IndexError("Index out of range")

        return rd

    '''
    Looks up a single byte from byte stream
    @return bytes   Byte looked up
    '''
    def lookup8(self):
        return self.lookup(1)

    '''
    Looks up two bytes from byte stream
    @return bytes   Bytes looked up
    '''
    def lookup16(self):
        return self.lookup(2)

    '''
    Lookus up a byte and unpacks it into an unsigned integer
    @return int 1-byte long unsigned integer
    '''
    def lookupUnpackUint8(self, endian=None):
        if endian is None:
            return int.from_bytes(self.lookup8(), byteorder=self.endian, signed=False)
        else:
            return int.from_bytes(self.lookup8(), byteorder=endian, signed=False)

    '''
    Lookus up two bytes and unpacks them into an unsigned integer
    @return int 2-bytes long unsigned integer
    '''
    def lookupUnpackUint16(self, endian=None):
        if endian is None:
            return int.from_bytes(self.lookup16(), byteorder=self.endian, signed=False)
        else:
            return int.from_bytes(self.lookup16(), byteorder=endian, signed=False)

    '''
    Appends data to the end of the stream
    @param  data    Binary data to be appended
    '''
    def append(self, data):
        self.byteStream += data

    '''
    Replaces data in the stream, overwriting what was stored in the place previously
    @param  idx     Index of the stream to be replace data
    @param  data    Binary data to be placed
    '''
    def replace(self, idx, data):
        self.byteStream = self.byteStream[:idx] + data + self.byteStream[idx+len(data):]


'''
Table of strings, represented as a dictionary where the keys are the offsets (addresses) of the strings in the file and the
values are the strings themselves
'''
class StringTable(dict):
    '''
    Constructs a StringTable object (dictionary of strings)
    @param  binReader    Binary reader to parse the table from
    '''
    def __init__(self, binReader):
        # Inherit all characteristics of a dictionary:
        super().__init__(self)

        # Get length, in bytes, of this field:
        self.binLen = binReader.unpackUint32()

        # Store every string and their respective offsets:
        offset = 0
        while offset < self.binLen:
            self[offset] = binReader.readString(self.binLen - offset - 1)
            offset += len(self[offset]) + 1

    '''
    Gets a string or substring of the table
    @param  key     Key of entry to be retrieved
    '''
    def __getitem__(self, key):
        # If out of bounds:
        if key < 0 or key >= self.binLen:
            raise KeyError

        try:
            # Try default dict operator:
            return super().__getitem__(key)
        except KeyError:
            # If null byte:
            if key + 1 in self:
                return b''

            # Decrease until a valid key is found:
            k = key
            while k >= 0 and k not in self:
                k -= 1

            if k < 0:
                raise KeyError
            else:
                # Return substring:
                return self[k][key-k:]

'''
Table of floating point numbers, represented as a list of floats
'''
class FloatTable(list):
    '''
    Constructs a FloatTable object (list of floats)
    @param  binReader    Binary reader to parse the table from
    '''
    def __init__(self, binReader):
        # Inherit all characteristics of a list:
        super().__init__(self)

        # Get length, in entries, of this field:
        length = binReader.unpackUint32()

        # Store floats:
        for _ in range(0, length):
            self.append(binReader.unpackFloat32())


'''
Code stream that constitutes the script, represented as a BinaryReader
'''
class ByteCode(binaryReading):
    '''
    Constructs a ByteCode object
    @param  binReader   Binary reader to parse the bytecode from
    @param  extCtrlCode Control code to indicate 2-bytes long code value
    @param  endCtrlCode Control code to indicate EOF
    '''
    def __init__(self, binReader, extCtrlCode=0xff, endCtrlCode=0xcdcd):
        # Inherit all characteristics of binary.Reading:
        super().__init__(b'', "little")

        self.extCtrlCode = extCtrlCode
        self.endCtrlCode = endCtrlCode

        self.extCtrlByte = bytes([extCtrlCode])

        # For indexing the bytecode by code:
        self.idxTable = [] # List of code indices
        self.codLen = binReader.unpackUint32() # Number of codes

        # Get start offset of bytecode:
        offset = binReader.pointer

        for _ in range(0, self.codLen):
            # Read byte:
            bt = binReader.read8()

            # If extension control code:
            if bt == self.extCtrlByte:
                # Get next 2 bytes as part of same code:
                bt += binReader.read16()

            # Append packed code to stream:
            self.append(bt)

            # Store the index of the code in the stream:
            self.idxTable.append(binReader.pointer - offset - 1)

        self.binLen = len(self.byteStream) # Number of bytes

    '''
    Retrieves the code currently pointed at
    '''
    def getCode(self):
        # Get code:
        code = self.unpackUint8()

        # If extension control code:
        if code == self.extCtrlCode:
            # Get the actual 2-bytes long code:
            code = self.unpackUint16()
        
        return code

    '''
    Retrieves next two bytes as uint
    '''
    def getUint(self):
        # If first byte is extension control code:
        if self.lookup8() == self.extCtrlByte:
            # Discard first byte:
            self.read8()
            # Get 2-bytes long stream:
            bt = self.read16()
            # Discard byte:
            self.read8()
            # Append more two bytes:
            bt += self.read16()
            # Get 4-bytes long big endian unsigned integer:
            return int.from_bytes(bt, byteorder="big", signed=False)
        # If second byte is extension control code:
        elif self.lookup16()[1] == self.extCtrlCode:
            # Discard first two bytes:
            self.read16()
            # Get 2-bytes long big endian unsigned integer:
            return self.unpackUint16(endian="big")
        else:
            # Get 2-bytes long big endian unsigned integer:
            return self.unpackUint16(endian="big")

    '''
    Retrieves next two bytes as string offset
    '''
    def getStringOffset(self):
        # If second byte is extension control code:
        if self.lookup16()[1] == self.extCtrlCode:
            # Discard two bytes:
            self.read16()
            # Get 2-bytes long little endian offset:
            return self.unpackUint16()
        # If patched string:
        elif self.pointer in self.patches:
            # Get 2-bytes long little endian offset:
            return self.unpackUint16()
        else:
            # Get 2-bytes long big endian offset:
            return self.unpackUint16(endian="big")

    '''
    Retrieves next two bytes as float offset
    '''
    def getFloatOffset(self):
        return self.unpackUint16(endian="big")

    '''
    Dump chunk of bytecode
    @param  start   Start byte index of chunk
    @param  end     End byte index of chunk
    '''
    def dump(self, start, end):
        return self.byteStream[start:end]

    '''
    Patches the string offsets into the locations listed in the IdentTable
    @param  identTable  IdentTable described in the file
    '''
    def patchStrings(self, identTable):
        self.patches = []
        for offset, locations in identTable.items():
            for loc in locations:
                self.replace(self.idxTable[loc], offset) # Patch location (code index)
                self.patches.append(self.idxTable[loc]) # List patched location (byte index)


'''
Identification Table that maps the strings to the opcode stream, represented as a dictionary where the keys are the offsets
of the strings and the values are lists of indices of the stream
'''
class IdentTable(dict):
    '''
    Constructs an IdentTable object (dictionary of lists of integers)
    @param  binReader   Binary reader to parse the table from
    '''
    def __init__(self, binReader):
        # Inherit all characteristics of a dictionary:
        super().__init__(self)

        # Get length, in entries, of this field:
        length = binReader.unpackUint32()

        # For each entry of the table:
        for _ in range(0, length):
            offset = binReader.read32()[:2] # Get offset field (offset is just 2-bytes long)
            count = binReader.unpackUint32() # Get count field
            offset = bytes(offset)
            self[offset] = []
            # For each location to patch:
            for _ in range(0, count):
                # Store location into "offset" entry:
                self[offset].append(binReader.unpackUint32())


'''
Raise this exception when something went wrong during parsing
'''
class ParsingError(Exception):
    '''
    Constructs a new ParsingError object
    @param  msg     Specific message to be appended on the beginning of error message
    @param  name    Name of the file being parsed that raised the exception
    '''
    def __init__(self, msg, name):
        self.message = msg + ". File \"" + name + "\" might be corrupted or encoded in a different standard"


'''
Raise this exception when a method that depends on the parsed file is called before parsing
'''
class NotParsedError(Exception):
    '''
    Constructs a new NotParsedError object
    @param  method  Method where the exception was raised
    '''
    def __init__(self, method):
        self.message = "File should be parsed before calling method \"" + method + "\""


'''
Structure of a DSO file and methods for parsing it
'''
class dsoFile():
    '''
    Constructs a File object
    @param  path    Path of file to be parsed
    '''
    def __init__(self, path):
        # Save file path:
        self.path = path

        # Save file name:
        self.name = path.name

        ## Dump contents to a binary reader:
  
        with open(path, "rb") as fd:
            self.binReader = binaryReading(fd.read(), "little")


        #else:
        #a_string = window.code
        #encoded_string = a_string.encode()
        #byte_array = bytearray(a_string)
        #self.binReader = binaryReading(byte_array, "little")


        # Not parsed yet:
        self.parsed = False

    '''
    Parses the file into tables and bytecode
    '''
    def parse(self):
        # Parse the version of script:
        self.version = self.binReader.unpackUint32()

        # Parse the Global String Table:
        self.globalStringTable = StringTable(self.binReader)

        # Parse the Global Float Table:
        self.globalFloatTable = FloatTable(self.binReader)

        # Parse the Function String Table:
        self.functionStringTable = StringTable(self.binReader)

        # Parse the Function Float Table:
        self.functionFloatTable = FloatTable(self.binReader)

        # Parse the ByteCode:
        self.byteCode = ByteCode(self.binReader)

        # Parse the Ident Table:
        self.identTable = IdentTable(self.binReader)

        if len(self.binReader.byteStream) != self.binReader.pointer:
            raise ParsingError("Parsing did not reach EOF", self.name)

        # Patch bytecode, resolving all references to strings:
        self.byteCode.patchStrings(self.identTable)

        self.parsed = True

    '''
    Dumps the structures of the parsed file
    @param  sink    Output to dump contents to
    '''
    def dump(self, sink=stdout):
        if not self.parsed:
            raise NotParsedError("dump")

        print("Script version: {}".format(self.version), file=sink)
        print("", file=sink)

        print("Global String Table:", file=sink)
        print(self.globalStringTable, file=sink)
        print("", file=sink)

        print("Global Float Table:", file=sink)
        print(self.globalFloatTable, file=sink)
        print("", file=sink)

        print("Function String Table:", file=sink)
        print(self.functionStringTable, file=sink)
        print("", file=sink)

        print("Function float Table:", file=sink)
        print(self.functionFloatTable, file=sink)
        print("", file=sink)

        print("Byte Code:", file=sink)
        print(self.byteCode.byteStream, file=sink)
        print("", file=sink)

        print("Byte Code indexes:", file=sink)
        print(self.byteCode.idxTable, file=sink)
        print("", file=sink)

        print("Ident Table:", file=sink)
        print(self.identTable, file=sink)
        print("", file=sink)

'''
Class for simulating the data structure used by Torque VM
'''
class StringStack(list):
    '''
    Loads a string into the top of the stack (does not change top pointer)
    @param  string  String to be loaded
    '''
    def load(self, string):
        if self:
            self[-1] = string
        else:
            self.append(string)

    '''
    Advances the top pointer to after the string currenty pointed at (push == load + advance)
    @param  ch  Character to be appended to the end of current string
    '''
    def advance(self, ch=None):
        if ch is not None:
            if ch == "\n":
                # Use Torque "NL" operator:
                self.append(ConcatNl([self.pop()]))
            elif ch == "\t":
                # Use Torque "TAB" operator:
                self.append(ConcatTab([self.pop()]))
            elif ch == " ":
                # Use Torque "SPC" operator:
                self.append(ConcatSpc([self.pop()]))
            elif ch == ",":
                # Separate current string and the one to be loaded with a comma:
                self.append(ConcatComma([self.pop()]))
            elif ch == "\x00":
                # Torque only separates strings in the stack with a null byte if they are going to be compared:
                self.append(StringEqual([self.pop()]))
            else:
                # Use Torque "@" operator:
                self.append(Concat([self.pop(), "\"" + ch + "\""]))

        # If no char is given, the strings being pushed are either for a concatenation, or for an array access
        # Method rewin takes care of the rest

        # To be overwritten by next load:
        self.append(None)

    '''
    Rewinds top of stack, joining the two strings at the top into a single structure at the top
    '''
    def rewind(self):
        s2 = self.pop()
        s1 = self.pop()

        # In summary:
        #   If the operation was defined in method advance, take the loaded string and append it to the operands
        #   If both are normal strings, operation is unknown at this point, so just join them in a list
        #   If at least one of them is a list (probably because of consecutive rewinds):
        #       Append to operands, if there is a concatenation operation involved
        #       Join everything into one single list otherwise

        if isinstance(s2, list):
            if isinstance(s1, list):
                self.append(s1 + s2) # list + list
            elif hasattr(s1, "isString") and s1.isString:
                s1.operands.append(Concat(s2)) # concat(x, concat(s2))
                self.append(s1)
            else:
                s2.insert(0, s1) # list(s1, x)
                self.append(s2)
        else:
            if isinstance(s1, list):
                s1.append(s2) # list(x, s2)
                self.append(s1)
            elif hasattr(s1, "isString") and s1.isString:
                s1.operands.append(s2) # concat(x, s2)
                self.append(s1)
            else:
                self.append([s1, s2]) # list(s1, s2)

    '''
    Writes null byte at the top (discarding whatever was there), then rewinds to previous top
    '''
    def terminateRewind(self):
        # Effectively a pop that discards the value:
        self.pop()


'''
Methods for decoding a DSO file's bytecode
'''
class codecDecoding:
    '''
    Constructs a Decoding object
    @param  dsoFile     Parsed dso.File object to be decoded
    @param  inFunction  Indicates if start is inside a function and at which depth (for partial decompilation only)
    @param  offset      Start offset of bytecode (for partial decompilation only)
    '''
    def __init__(self, dsoFile, inFunction=0, offset=0):
        self.file = dsoFile
        self.inFunction = inFunction
        self.offset = offset

        # For storing current variable, object and field:
        self.curvar = None
        self.curobj = None
        self.curfield = None

        # Arument frame for function calls and object creations:
        self.argFrame = []

        # Stacks for each data type:
        self.binStack = []
        self.intStack = []
        self.fltStack = []
        self.strStack = StringStack()

        # Stack of Tree:
        self.treeStack = []

        # Main tree of file:
        self.tree = Tree(torqueFile(self.file.name))

        # Stack of opcode calls performed:
        self.callStack = []

        # Instruction pointer:
        self.ip = 0

        # Dictionary for storing which addresses mark the end of code blocks and from which syntatic structures:
        self.endBlock = {}

    '''
    Retrieves next code of bytecode
    '''
    def getCode(self):
        return self.file.byteCode.getCode()

    '''
    Retrieves next data of bytecode as uint
    '''
    def getUint(self):
        return self.file.byteCode.getUint()

    '''
    Retrieves next data of bytecode as string offset
    '''
    def getStringOffset(self):
        return self.file.byteCode.getStringOffset()

    '''
    Retrieves string of Global String Table
    @param  offset      Offset of string to be retrieved
    @param  encoding    Encoding of the string
    '''
    def getGlobalStringByOffset(self, offset, encoding="utf-8"):
        return self.file.globalStringTable[offset].decode(encoding)

    '''
    Retrieves next data of bytecode as string offset then retrieves global string at that offset
    @param  encoding    Encoding of the string
    '''
    def getGlobalString(self, encoding="utf-8"):
        return self.getGlobalStringByOffset(self.getStringOffset(), encoding)

    '''
    Retrieves string of Function String Table
    @param  offset      Offset of string to be retrieved
    @param  encoding    Encoding of the string
    '''
    def getFunctionStringByOffset(self, offset, encoding="utf-8"):
        return self.file.functionStringTable[offset].decode(encoding)

    '''
    Retrieves next data of bytecode as string offset then retrieves function string at that offset
    @param  encoding    Encoding of the string
    '''
    def getFunctionString(self, encoding="utf-8"):
        return self.getFunctionStringByOffset(self.getStringOffset(), encoding)

    '''
    Retrieves string of string table accordingly to the context
    @param  offset      Offset of string to be retrieved
    @param  encoding    Encoding of the string
    '''
    def getStringByOffset(self, offset, encoding="utf-8"):
        if self.inFunction and self.file.functionStringTable:
            return self.getFunctionStringByOffset(offset, encoding)
        else:
            return self.getGlobalStringByOffset(offset, encoding)

    '''
    Retrieves next data of bytecode as string offset then retrieves string at that offset
    @param  encoding    Encoding of the string
    '''
    def getString(self, encoding="utf-8"):
        return self.getStringByOffset(self.getStringOffset(), encoding)

    '''
    Replaces global string at given offset by given string
    @param  offset      Offset of string to be set
    @param  string      String to replace with
    @param  encoding    Encoding of the string
    '''
    def setGlobalString(self, offset, string, encoding="utf-8"):
        self.file.globalStringTable[offset] = string.encode(encoding)

    '''
    Replaces function string at given offset by given string
    @param  offset      Offset of string to be set
    @param  string      String to replace with
    @param  encoding    Encoding of the string
    '''
    def setFunctionString(self, offset, string, encoding="utf-8"):
        self.file.functionStringTable[offset] = string.encode(encoding)

    '''
    Replaces string at given offset by given string
    @param  offset      Offset of string to be set
    @param  string      String to replace with
    @param  encoding    Encoding of the string
    '''
    def setString(self, offset, string, encoding="utf-8"):
        if self.inFunction and self.file.functionStringTable:
            return self.setFunctionString(offset, string, encoding)
        else:
            return self.setGlobalString(offset, string, encoding)

    '''
    Retrieves next data of bytecode as float offset
    '''
    def getFloatOffset(self):
        return self.file.byteCode.getFloatOffset()

    '''
    Retrieves float of Global Float Table
    @param  offset  Offset of float to be retrieved
    '''
    def getGlobalFloatByOffset(self, offset):
        return self.file.globalFloatTable[offset]

    '''
    Retrieves next data of bytecode as float offset then retrieves global float at that offset
    '''
    def getGlobalFloat(self):
        return self.getGlobalFloatByOffset(self.getFloatOffset())

    '''
    Retrieves float of Function Float Table
    @param  offset  Offset of float to be retrieved
    '''
    def getFunctionFloatByOffset(self, offset):
        return self.file.functionFloatTable[offset]

    '''
    Retrieves next data of bytecode as float offset then retrieves function float at that offset
    '''
    def getFunctionFloat(self):
        return self.getFunctionFloatByOffset(self.getFloatOffset())

    '''
    Retrieves float of float table accordingly to the context
    @param  offset  Offset of float to be retrieved
    '''
    def getFloatByOffset(self, offset):
        if self.inFunction and self.file.functionFloatTable:
            return self.getFunctionFloatByOffset(offset)
        else:
            return self.getGlobalFloatByOffset(offset)

    '''
    Retrieves next data of bytecode as float offset then retrieves float at that offset
    '''
    def getFloat(self):
        return self.getFloatByOffset(self.getFloatOffset())

    '''
    Converts given code index into byte index
    @param  code    Code index to be converted
    '''
    def getByteIndex(self, code):
        return self.file.byteCode.idxTable[code]

    '''
    Retrieves index of next byte to be read
    '''
    def getCurByteIndex(self):
        return self.file.byteCode.pointer

    '''
    Records Torque code block structure and its end address
    @param  key     Offset of end of code block (key of dictionary)
    @param  val     core.torque object which contains the code block (value to be appended)
    '''
    def recordEndOfBlock(self, key, val):
        if key not in self.endBlock:
            self.endBlock[key] = []

        self.endBlock[key].append(val)

    '''
    Dumps bytecode of current instruction (assuming all its parameters have been read already)
    '''
    def dumpInstruction(self):
        return self.file.byteCode.dump(self.ip, self.file.byteCode.pointer)

    '''
    Routine called for OP_FUNC_DECL (declare a function)

    Instantiates a FuncDecl object, appends it to the tree and focuses on it
    '''
    def opFuncDecl(self):
        # Get function name:
        funcName = self.getGlobalString()

        # Get function namespace:
        offset = self.getStringOffset()
        namespace = self.getGlobalStringByOffset(offset) if offset else ""

        # Get function package:
        # TODO: Should we use this somewhere?
        package = self.getGlobalString()

        # Get boolean indicating if function has body:
        # TODO: Should we use this somewhere?
        hasBody = self.getCode()

        # Get end of function declaration (first instruction after its end):
        end = self.getByteIndex(self.getCode())

        # Get argc of function (number of arguments):
        argc = self.getCode()

        # Get argv of function (list of arguments):
        argv = []
        for _ in range(0, argc):
            offset = self.getStringOffset()
            string = self.getGlobalStringByOffset(offset)

            # Mark parameter as local variable if not already done:
            if string[0] != "%":
                self.setGlobalString(offset, "%" + string)

            argv.append(string)

        # Instantiate object:
        decl = FuncDecl(funcName, namespace, package, hasBody, end, argc, argv)

        # Store end of declaration block:
        self.recordEndOfBlock(end, decl)

        # Append object to tree:
        self.tree.append(decl)

        # Focus on object (next instructions are in the scope of this declaration):
        self.tree.focusChild()

        # Indicate flow has entered another function:
        self.inFunction += 1

        logging.debug("IP: {}: {}: Declare function: {}, {}, {}, {}, {}, {}".format(self.ip, self.dumpInstruction(), funcName, namespace, package, hasBody, end, argc))

    '''
    Routine called for OP_CREATE_OBJECT (create an an object)

    Instantiates a ObjCreation object, creates a new tree and appends it to the root
    '''
    def opCreateObject(self):
        # Get parent object:
        parent = self.getString()

        # TODO: This is a mistery because I actually do not know what it is
        mistery = self.getCode()

        # Get end of object creation (first instruction after its end):
        end = self.getByteIndex(self.getCode())

        # Get argv of object (list of arguments):
        argv = self.argFrame.pop()

        for string in argv[1:]:
            # Mark parameter as local variable if not already done:
            if isinstance(string, str) and string[0] != "%":
                string = "%" + string

        # Store current tree on stack:
        self.treeStack.append(self.tree)

        # Instantiate object and add it to the root of a new tree:
        self.tree = Tree(ObjCreation(parent, mistery, argv))

        logging.debug("IP: {}: {}: Create object: {}, {}, {}".format(self.ip, self.dumpInstruction(), parent, mistery, end))

    '''
    Routine called for OP_ADD_OBJECT (add object to stack)

    Adds object to the int stack
    '''
    def opAddObject(self):
        # Get boolean value:
        placeAtRoot = self.getCode()

        if placeAtRoot:
            # Overwrite value pushed before its creation by it:
            self.intStack[-1] = self.tree.root
        else:
            # Push it to stack:
            self.intStack.append(self.tree.root)

        logging.debug("IP: {}: {}: Add object".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_END_OBJECT (end object creation)

    Restores previous tree
    '''
    def opEndObject(self):
        # Restore previous tree:
        self.tree = self.treeStack.pop()

        # If object has no body:
        if self.callStack[-2] is codecDecoding.opCreateObject and self.callStack[-1] is codecDecoding.opAddObject:
            # Do not create a block of code:
            self.intStack[-1].block = 0

        # Get boolean value:
        placeAtRoot = self.getCode()

        if not placeAtRoot:
            # Append the object to the tree already since it will not be assigned to anything:
            self.tree.append(self.intStack.pop())

        logging.debug("IP: {}: {}: End object".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_JMPIFFNOT (jump if condition retrieved from float stack not met)

    Appends a If if its a forward jump or updates/replaces current node by a While if it is a backward jump
    '''
    def opJmpiffnot(self):
        # Get jump target:
        target = self.getByteIndex(self.getCode()) - self.offset

        # Get branch condition:
        if self.binStack:
            condition = self.binStack.pop()
            condition.operands.append(self.fltStack.pop())
        else:
            condition = self.fltStack.pop()

        # If forward jump:
        if target > self.getCurByteIndex():
            # Assume it is an If:
            statement = If(condition)

            # Store target as end of If block:
            self.recordEndOfBlock(target, statement)

            # Append object to tree:
            self.tree.append(statement)

            # Focus on it:
            self.tree.focusChild()
        # If backward jump:
        elif target < self.getCurByteIndex():
            if isinstance(self.tree.getFocused(), While):
                # If while statement was compiled as unconditional jump + conditional jump, condition is not set by now:
                self.tree.getFocused().condition = Not([condition])
            else:
                # If while statement was compiled as conditional jump + conditional jump, it was assumed it was an if:
                if condition != self.tree.getFocused().condition:
                    raise ValueError("Loop condition mismatch: {}: {}: {}".format(condition, self.tree.getFocused().condition, target))

                # Replace if by while statement (invert condition):
                self.tree.replace(While(Not([condition])))

        logging.debug("IP: {}: {}: Jump if float condition not met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMPIFNOT (jump if condition retrieved from int stack not met)

    Appends a If if its a forward jump or updates/replaces current node by a While if it is a backward jump
    '''
    def opJmpifnot(self):
        # Get jump target:
        target = self.getByteIndex(self.getCode()) - self.offset

        # Get branch condition:
        if self.binStack:
            condition = self.binStack.pop()
            condition.operands.append(self.intStack.pop())
        else:
            condition = self.intStack.pop()

        # If forward jump:
        if target > self.getCurByteIndex():
            # Assume it is an If:
            statement = If(condition)

            # Store target as end of If block:
            self.recordEndOfBlock(target, statement)

            # Append object to tree:
            self.tree.append(statement)

            # Focus on it:
            self.tree.focusChild()
        # If backward jump:
        elif target < self.getCurByteIndex():
            if isinstance(self.tree.getFocused(), While):
                # If while statement was compiled as unconditional jump + conditional jump, condition is not set by now:
                self.tree.getFocused().condition = Not([condition])
            else:
                # If while statement was compiled as conditional jump + conditional jump, it was assumed it was an if:
                if condition != self.tree.getFocused().condition:
                    raise ValueError("Loop condition mismatch: {}: {}: {}".format(condition, self.tree.getFocused().condition, target))

                # Replace if by while statement (invert condition):
                self.tree.replace(While(Not([condition])))

        logging.debug("IP: {}: {}: Jump if uint/boolean condition not met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMPIFF (jump if condition retrieved from float stack met)

    Appends a If if its a forward jump or updates/replaces current node by a While if it is a backward jump
    '''
    def opJmpiff(self):
        # Get jump target:
        target = self.getByteIndex(self.getCode()) - self.offset

        # Get branch condition:
        if self.binStack:
            condition = self.binStack.pop()
            condition.operands.append(self.fltStack.pop())
        else:
            condition = self.fltStack.pop()

        # If forward jump:
        if target > self.getCurByteIndex():
            # Assume it is an If:
            statement = If(Not([condition]))

            # Store target as end of If block:
            self.recordEndOfBlock(target, statement)

            # Append object to tree:
            self.tree.append(statement)

            # Focus on it:
            self.tree.focusChild()
        # If backward jump:
        elif target < self.getCurByteIndex():
            if isinstance(self.tree.getFocused(), While):
                # If while statement was compiled as unconditional jump + conditional jump, condition is not set by now:
                self.tree.getFocused().condition = condition
            else:
                # If while statement was compiled as conditional jump + conditional jump, it was assumed it was an if:
                if condition != self.tree.getFocused().condition:
                    raise ValueError("Loop condition mismatch: {}: {}: {}".format(condition, self.tree.getFocused().condition, target))

                # Replace if by while statement:
                self.tree.replace(While(condition))

        logging.debug("IP: {}: {}: Jump if float condition not met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMPIF (jump if condition retrieved from int stack met)

    Appends a If if its a forward jump or updates/replaces current node by a While if it is a backward jump
    '''
    def opJmpif(self):
        # Get jump target:
        target = self.getByteIndex(self.getCode()) - self.offset

        # Get branch condition:
        if self.binStack:
            condition = self.binStack.pop()
            condition.operands.append(self.intStack.pop())
        else:
            condition = self.intStack.pop()

        # If forward jump:
        if target > self.getCurByteIndex():
            # Assume it is an If:
            statement = If(Not([condition]))

            # Store target as end of If block:
            self.recordEndOfBlock(target, statement)

            # Append object to tree:
            self.tree.append(statement)

            # Focus on it:
            self.tree.focusChild()
        # If backward jump:
        elif target < self.getCurByteIndex():
            if isinstance(self.tree.getFocused(), While):
                # If while statement was compiled as unconditional jump + conditional jump, condition is not set by now:
                self.tree.getFocused().condition = condition
            else:
                # If while statement was compiled as conditional jump + conditional jump, it was assumed it was an if:
                if condition != self.tree.getFocused().condition:
                    raise ValueError("Loop condition mismatch: {}: {}: {}".format(condition, self.tree.getFocused().condition, target))

                # Replace if by while statement:
                self.tree.replace(While(condition))

        logging.debug("IP: {}: {}: Jump if uint/boolean condition met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMP (jump unconditionally)

    Appends either a Else, a Break or a While to the tree
    '''
    def opJmp(self):
        target = self.getByteIndex(self.getCode()) - self.offset

        # If forward jump:
        if target > self.getCurByteIndex():
            # If the current code block ends on this instruction:
            if self.getCurByteIndex() in self.endBlock and self.tree.getFocused() in self.endBlock[self.getCurByteIndex()]:
                # If the target is right after the end of a block, it is a break from a loop:
                if target in self.endBlock:
                    statement = Break()
                    self.tree.append(statement)
                # Otherwise, it is an else:
                else:
                    statement = Else()
                    self.recordEndOfBlock(target, statement)
                    # Associate to the current if statement
                    self.tree.getFocused().elseHandle = statement
            else:
                # Since it is an unconditional branch, condition is unknown at this point:
                statement = While("")
                self.recordEndOfBlock(target, statement)
                self.tree.append(statement)
                self.tree.focusChild()
        else:
            raise NotImplementedError("Backward jump not implemented for OP_JMP")

        logging.debug("IP: {}: {}: Jump to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMPIFNOT_NP (jump if boolean condition not met - I believe this is used for && operation short-circuit)

    Retrieves a boolean condition from the int stack and appends it as operand of a And operation
    '''
    def opJmpifnotNp(self):
        target = self.getByteIndex(self.getCode()) - self.offset

        if self.binStack:
            # Get previous condition:
            previousCond = self.binStack.pop()
            previousCond.operands.append(self.intStack.pop())
            self.binStack.append(And([previousCond]))
        else:
            self.binStack.append(And([self.intStack.pop()]))

        logging.debug("IP: {}: {}: Jump if binary condition not met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_JMPIF_NP (jump if boolean condition met - I believe this is used for || operation short-circuit)

    Retrieves a boolean condition from the int stack and appends it as operand of a Or operation
    '''
    def opJmpifNp(self):
        target = self.getByteIndex(self.getCode()) - self.offset

        if self.binStack:
            # Get previous condition:
            previousCond = self.binStack.pop()
            previousCond.operands.append(self.intStack.pop())
            self.binStack.append(Or([previousCond]))
        else:
            self.binStack.append(Or([self.intStack.pop()]))

        logging.debug("IP: {}: {}: Jump if binary condition met to: {}".format(self.ip, self.dumpInstruction(), target))

    '''
    Routine called for OP_RETURN (return from function/script)

    Appends a Return object to the tree
    '''
    def opReturn(self):
        # If a return value was loaded:
        if self.strStack:
            ret = self.strStack[-1]
        else:
            ret = None

        self.tree.append(Return(ret))

        logging.debug("IP: {}: {}: Return".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPEQ (compare if floats are equal)

    Appends a Equal operation to the int stack
    '''
    def opCmpeq(self):
        self.intStack.append(Equal([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if equal".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPLT (compare if a float is less than another one)

    Appends a Less operation to the int stack
    '''
    def opCmplt(self):
        self.intStack.append(Less([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if less".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPLE (compare if a float is less than or equal to another one)

    Appends a LessOrEqual operation to the int stack
    '''
    def opCmple(self):
        self.intStack.append(LessOrEqual([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if less or equal".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPGR (compare if a float is greater than another one)

    Appends a Greater operation to the int stack
    '''
    def opCmpgr(self):
        self.intStack.append(Greater([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if greater".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPGE (compare if a float is greater than or equal to another one)

    Appends a GreaterOrEqual operation to the int stack
    '''
    def opCmpge(self):
        self.intStack.append(GreaterOrEqual([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if greater or equal".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_CMPNE (compare if a float is not equal to another one)

    Appends a NotEqual operation to the int stack
    '''
    def opCmpne(self):
        self.intStack.append(NotEqual([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Compare if not equal".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_XOR (perform a bitwise xor operation between two integers)

    Appends a Xor operation to the int stack
    '''
    def opXor(self):
        self.intStack.append(Xor([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Bitwise xor".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_MOD (perform a modulo operation between two integers)

    Appends a Mod operation to the int stack
    '''
    def opMod(self):
        self.intStack.append(Mod([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Get modulo".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_BITAND (perform a bitwise and operation between two integers)

    Appends a BitAnd operation to the int stack
    '''
    def opBitand(self):
        self.intStack.append(BitAnd([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Bitwise and".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_BITOR (perform a bitwise or operation between two integers)

    Appends a BitOr operation to the int stack
    '''
    def opBitor(self):
        self.intStack.append(BitOr([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Bitwise or".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_NOT (negate an integer)

    Appends a Not operation to the int stack
    '''
    def opNot(self):
        self.intStack.append(Not([self.intStack.pop()]))

        logging.debug("IP: {}: {}: Negate uint/boolean value".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_NOTF (negate a float)

    Appends a Not operation to the int stack
    '''
    def opNotf(self):
        self.intStack.append(Not([self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Negate float value".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_ONESCOMPLEMENT (perform an one's complement operation between two integers)

    Appends a Complement operation to the int stack
    '''
    def opOnescomplement(self):
        self.intStack.append(Complement([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Bitwise complement".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SHR (shift an integer to the right by another integer value)

    Appends a ShiftRight operation to the int stack
    '''
    def opShr(self):
        self.intStack.append(ShiftRight([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Shift right".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SHL (shift an integer to the left by another integer value)

    Appends a ShiftLeft operation to the int stack
    '''
    def opShl(self):
        self.intStack.append(ShiftLeft([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Shift left".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_AND (perform a boolean and operation between two integers)

    Appends a And operation to the int stack
    '''
    def opAnd(self):
        self.intStack.append(And([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Logical and".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_OR (perform a boolean or operation between two integers)

    Appends a Or operation to the int stack
    '''
    def opOr(self):
        self.intStack.append(Or([self.intStack.pop(), self.intStack.pop()]))

        logging.debug("IP: {}: {}: Logical or".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_ADD (add two floats)

    Appends a Add operation to the float stack
    '''
    def opAdd(self):
        self.fltStack.append(Add([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Sum floats".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SUB (subtract two floats)

    Appends a Sub operation to the float stack
    '''
    def opSub(self):
        self.fltStack.append(Sub([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Subtract floats".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_MUL (multiply two floats)

    Appends a Mul operation to the float stack
    '''
    def opMul(self):
        self.fltStack.append(Mul([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Multiply floats".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_DIV (divide one float by another)

    Appends a Div operation to the float stack
    '''
    def opDiv(self):
        self.fltStack.append(Div([self.fltStack.pop(), self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Divide floats".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_NEG (invert the sign of a float)

    Appends a Neg operation to the float stack
    '''
    def opNeg(self):
        self.fltStack.append(Neg([self.fltStack.pop()]))

        logging.debug("IP: {}: {}: Invert sign of float".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SETCURVAR and OP_SETCURVAR_CREATE (set name/symbol of current variable)

    Retrieves the symbol from the Global String Table and sets current variable to it
    '''
    def opSetcurvar(self):
        offset = self.getStringOffset()
        string = self.getGlobalStringByOffset(offset)

        if string[0] != "$" and string[0] != "%":
            if self.inFunction:
                # Append local variable prefix:
                string = "%" + string
            else:
                # Append global variable prefix:
                string = "$" + string

            # Save change:
            self.setGlobalString(offset, string)

        self.curvar = string

        logging.debug("IP: {}: {}: Set current variable: {}".format(self.ip, self.dumpInstruction(), self.curvar))

    '''
    Routine called for OP_SETCURVAR_ARRAY and OP_SETCURVAR_ARRAY_CREATE (set name/symbol and index of current array variable)

    Retrieves the symbol and index from string stack and append them to tree as ArrayAccess
    '''
    def opSetcurvarArray(self):
        self.curvar = ArrayAccess(self.strStack[-1])

        logging.debug("IP: {}: {}: Set current array variable".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_LOADVAR_UINT (load current variable value to int stack)

    Appends current variable symbol to the int stack
    '''
    def opLoadvarUint(self):
        self.intStack.append(self.curvar)

        logging.debug("IP: {}: {}: Load variable of type uint: {}".format(self.ip, self.dumpInstruction(), self.curvar))

    '''
    Routine called for OP_LOADVAR_FLT (load current variable value to float stack)

    Appends current variable symbol to the float stack
    '''
    def opLoadvarFlt(self):
        self.fltStack.append(self.curvar)

        logging.debug("IP: {}: {}: Load variable of type float: {}".format(self.ip, self.dumpInstruction(), self.curvar))

    '''
    Routine called for OP_LOADVAR_STR (load current variable value to string stack)

    Appends current variable symbol to the string stack
    '''
    def opLoadvarStr(self):
        self.strStack.load(self.curvar)

        logging.debug("IP: {}: {}: Load variable of type string: {}".format(self.ip, self.dumpInstruction(), self.curvar))

    '''
    Routine called for OP_SAVEVAR_UINT (save uint value into current variable)

    Retrieves the value from int stack and appends a Assignment object to the tree
    '''
    def opSavevarUint(self):
        name = self.curvar
        value = self.intStack[-1]

        self.curvar = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save uint value into variable".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SAVEVAR_FLT (save float value into current variable)

    Retrieves the value from float stack and appends a Assignment object to the tree
    '''
    def opSavevarFlt(self):
        name = self.curvar
        value = self.fltStack[-1]

        self.curvar = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save float value into variable".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SAVEVAR_STR (save string value into current variable)

    Retrieves the value from string stack and appends a Assignment object to the tree
    '''
    def opSavevarStr(self):
        name = self.curvar
        value = self.strStack[-1]

        if isinstance(value, list):
            value = Concat(value)

        self.curvar = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save string value into variable".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SETCUROBJECT (set name/symbol of current object)

    Retrieves the symbol from the string stack and sets current object to it
    '''
    def opSetcurobject(self):
        self.curobj = self.strStack[-1]

        logging.debug("IP: {}: {}: Set current object".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SETCUROBJECT_NEW (unset name/symbol of current object)

    Sets current object to None
    '''
    def opSetcurobjectNew(self):
        self.curobj = None

        logging.debug("IP: {}: {}: Set new current object".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SETCURFIELD (set name/symbol of current object field)

    Retrieves the symbol from the Global String Table and sets current field to it
    '''
    def opSetcurfield(self):
        self.curfield = self.getGlobalString()

        logging.debug("IP: {}: {}: Set current field: {}".format(self.ip, self.dumpInstruction(), self.curfield))

    '''
    Routine called for OP_SETCURVAR_ARRAY (set name/symbol and index of current array field)

    Retrieves the symbol and index from string stack and append them to tree as ArrayAccess
    '''
    def opSetcurfieldArray(self):
        self.curfield = ArrayAccess([self.curfield, self.strStack[-1]])

        logging.debug("IP: {}: {}: Set current array field".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_LOADFIELD_UINT (load current field value to int stack)

    Appends current field symbol to the int stack
    '''
    def opLoadfieldUint(self):
        self.intStack.append(self.curfield)

        logging.debug("IP: {}: {}: Load field of type string: {}".format(self.ip, self.dumpInstruction(), self.curfield))

    '''
    Routine called for OP_LOADFIELD_FLT (load current field value to float stack)

    Appends current field symbol to the float stack
    '''
    def opLoadfieldFlt(self):
        self.fltStack.append(self.curfield)

        logging.debug("IP: {}: {}: Load field of type string: {}".format(self.ip, self.dumpInstruction(), self.curfield))

    '''
    Routine called for OP_LOADFIELD_STR (load current field value to string stack)

    Appends current field symbol to the string stack
    '''
    def opLoadfieldStr(self):
        self.strStack.load(self.curfield)

        logging.debug("IP: {}: {}: Load field of type string: {}".format(self.ip, self.dumpInstruction(), self.curfield))

    '''
    Routine called for OP_SAVEFIELD_UINT (save uint value into current field)

    Retrieves the value from int stack and appends a Assignment object to the tree
    '''
    def opSavefieldUint(self):
        if self.curobj is None:
            # Assignment to field during object creation:
            name = self.curfield
        else:
            # Assignment to field of current object:
            name = FieldAccess([self.curobj, self.curfield])

        value = self.intStack[-1]

        self.curfield = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save uint value into field".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SAVEFIELD_FLT (save float value into current field)

    Retrieves the value from float stack and appends a Assignment object to the tree
    '''
    def opSavefieldFlt(self):
        if self.curobj is None:
            # Assignment to field during object creation:
            name = self.curfield
        else:
            # Assignment to field of current object:
            name = FieldAccess([self.curobj, self.curfield])

        value = self.fltStack[-1]

        self.curfield = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save float value into field".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_SAVEFIELD_STR (save string value into current field)

    Retrieves the value from string stack and appends a Assignment object to the tree
    '''
    def opSavefieldStr(self):
        if self.curobj is None:
            # Assignment to field during object creation:
            name = self.curfield
        else:
            # Assignment to field of current object:
            name = FieldAccess([self.curobj, self.curfield])

        value = self.strStack[-1]

        self.curfield = (name, value)

        self.tree.append(Assignment(name, value))

        logging.debug("IP: {}: {}: Save string value into field".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_STR_TO_UINT (convert string to uint)

    Pushes popped value from string stack into int stack
    '''
    def opStrToUint(self):
        self.intStack.append(self.strStack[-1])

        logging.debug("IP: {}: {}: Pop string into uint stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_STR_TO_FLT (convert string to float)

    Pushes popped value from string stack into float stack
    '''
    def opStrToFlt(self):
        self.fltStack.append(self.strStack[-1])

        logging.debug("IP: {}: {}: Pop string into float stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_STR_TO_NONE (discard string on top of stack)

    Pops top of string stack out
    '''
    def opStrToNone(self):
        popd = self.strStack.pop()

        # Return value being discarded means procedure call:
        if isinstance(popd, FuncCall) and self.callStack[-1] is codecDecoding.opCallfunc:
            self.tree.append(popd)

        logging.debug("IP: {}: {}: Pop string out".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_FLT_TO_UINT (convert float to uint)

    Pushes popped value from float stack into int stack
    '''
    def opFltToUint(self):
        self.intStack.append(self.fltStack.pop())

        logging.debug("IP: {}: {}: Pop float into uint stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_FLT_TO_STR (convert float to string)

    Pushes popped value from float stack into string stack
    '''
    def opFltToStr(self):
        self.strStack.load(self.fltStack.pop())

        logging.debug("IP: {}: {}: Pop float into string stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_FLT_TO_NONE (discard float on top of stack)

    Pops top of float stack out
    '''
    def opFltToNone(self):
        self.fltStack.pop()

        logging.debug("IP: {}: {}: Pop float out".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_UINT_TO_FLT (convert uint to float)

    Pushes popped value from uint stack into float stack
    '''
    def opUintToFlt(self):
        self.fltStack.append(self.intStack.pop())

        logging.debug("IP: {}: {}: Pop uint into float stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_UINT_TO_STR (convert uint to string)

    Pushes popped value from uint stack into string stack
    '''
    def opUintToStr(self):
        # If there is a concatenation of boolean conditions in progress:
        if self.binStack:
            condition = self.binStack.pop()
            condition.operands.append(self.intStack.pop())
        else:
            condition = self.intStack.pop()

        self.strStack.load(condition)

        logging.debug("IP: {}: {}: Pop uint into string stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_UINT_TO_NONE (discard uint on top of stack)

    Pops top of uint stack out
    '''
    def opUintToNone(self):
        popd = self.intStack.pop()

        # Return value being discarded means object declared, but not assigned:
        if isinstance(popd, ObjCreation) and self.callStack[-1] is codecDecoding.opEndObject:
            self.tree.append(popd)

        logging.debug("IP: {}: {}: Pop uint out".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_LOADIMMED_UINT (load uint into stack)

    Appends uint to stack
    '''
    def opLoadimmedUint(self):
        self.intStack.append(self.getUint())

        logging.debug("IP: {}: {}: Load uint: {}".format(self.ip, self.dumpInstruction(), self.intStack[-1]))

    '''
    Routine called for OP_LOADIMMED_FLT (load float into stack)

    Appends float to stack
    '''
    def opLoadimmedFlt(self):
        self.fltStack.append(self.getFloat())

        logging.debug("IP: {}: {}: Load float: {}".format(self.ip, self.dumpInstruction(), self.fltStack[-1]))

    '''
    Routine called for OP_LOADIMMED_STR (load string into stack)

    Appends string to stack
    '''
    def opLoadimmedStr(self):
        offset = self.getStringOffset()

        # Escaping might fail on some cases:
        try:
            string = "\"" + self.getStringByOffset(offset, "unicode_escape") + "\""
        except UnicodeDecodeError:
            string = "\"" + self.getStringByOffset(offset) + "\""

        self.strStack.load(string)

        logging.debug("IP: {}: {}: Load string: {}".format(self.ip, self.dumpInstruction(), self.strStack[-1]))

    '''
    Routine called for OP_LOADIMMED_IDENT (load "ident" (string) into stack)

    Appends string to stack
    '''
    def opLoadimmedIdent(self):
        offset = self.getStringOffset()

        # Escaping might fail on some cases:
        try:
            string = "\"" + self.getStringByOffset(offset, "unicode_escape") + "\""
        except UnicodeDecodeError:
            string = "\"" + self.getStringByOffset(offset) + "\""

        self.strStack.load(string)

        logging.debug("IP: {}: {}: Load string (ident): {}".format(self.ip, self.dumpInstruction(), self.strStack[-1]))

    '''
    Routine called for OP_TAG_TO_STR (load "tagged" string into stack)

    Appends (global) string to stack
    '''
    def opTagToStr(self):
        self.strStack.load(self.getGlobalString())

        logging.debug("IP: {}: {}: Load tagged string: {}".format(self.ip, self.dumpInstruction(), self.strStack[-1]))

    '''
    Routine called for OP_CALLFUNC and OP_CALLFUNC_RESOLVE (call function)

    Appends FuncCall object to the tree
    '''
    def opCallfunc(self):
        funcName = self.getGlobalString()

        offset = self.getStringOffset()

        if offset == 0:
            # Just read the place reserved for offset and do nothing:
            namespace = ""
        else:
            namespace = self.getGlobalStringByOffset(offset)

        callType = self.getCode()

        argv = self.argFrame.pop()

        for i in range(0, len(argv)):
            if isinstance(argv[i], list):
                argv[i] = Concat(argv[i])

        self.strStack.load(FuncCall(funcName, namespace, callType, argv))

        logging.debug("IP: {}: {}: Call function: {}, {}, {}".format(self.ip, self.dumpInstruction(), funcName, namespace, callType))

    '''
    Routine called for OP_ADVANCE_STR (advance top pointer of stack to end of top string)

    Calls method advance from string stack
    '''
    def opAdvanceStr(self):
        self.strStack.advance()

        logging.debug("IP: {}: {}: Advance string on stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_ADVANCE_STR_APPENDCHAR (advance top pointer of stack to end of top string and append character)

    Calls method advance from string stack
    '''
    def opAdvanceStrAppendchar(self):
        self.strStack.advance(chr(self.getCode()))

        logging.debug("IP: {}: {}: Advance string on stack and append char".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_ADVANCE_STR_COMMA (advance top pointer of stack to end of top string and append comma)

    Calls method advance from string stack
    '''
    def opAdvanceStrComma(self):
        self.strStack.advance(",")

        logging.debug("IP: {}: {}: Advance string on stack and append comma".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_ADVANCE_STR_NUL (advance top pointer of stack to end of top string and append null byte)

    Calls method advance from string stack
    '''
    def opAdvanceStrNul(self):
        self.strStack.advance("\x00")

        logging.debug("IP: {}: {}: Advance string on stack (null)".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_REWIND_STR (rewind top pointer of stack to previous element)

    Calls method rewind from string stack
    '''
    def opRewindStr(self):
        self.strStack.rewind()

        logging.debug("IP: {}: {}: Rewind string stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_TERMINATE_REWIND_STR (discard top string then rewind top pointer of stack to previous element)

    Calls method terminateRewind from string stack
    '''
    def opTerminateRewindStr(self):
        self.strStack.terminateRewind()

        logging.debug("IP: {}: {}: Terminate and rewind string stack".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_COMPARE_STR (compare two strings)

    Retrieves StringEqual operation from string stack and appends it to int stack
    '''
    def opCompareStr(self):
        s2 = self.strStack.pop()
        op = self.strStack.pop()

        op.operands.append(s2)

        self.intStack.append(op)

        logging.debug("IP: {}: {}: Compare strings".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_PUSH (push string into argument frame)

    Appends string from stack to argument frame
    '''
    def opPush(self):
        self.argFrame[-1].append(self.strStack[-1])

        logging.debug("IP: {}: {}: Push string to argument frame".format(self.ip, self.dumpInstruction()))

    '''
    Routine called for OP_PUSH_FRAME (push new argument frame)

    Appends empty list to list of argument frames
    '''
    def opPushFrame(self):
        self.argFrame.append([])

        logging.debug("IP: {}: {}: Push empty argument frame".format(self.ip, self.dumpInstruction()))

    '''
    Dictionary of calls by opcode
    '''
    callOp = {
        0:      opFuncDecl,
        1:      opCreateObject,


        4:      opAddObject,
        5:      opEndObject,
        6:      opJmpiffnot,
        7:      opJmpifnot,
        8:      opJmpiff,
        9:      opJmpif,
        10:     opJmpifnotNp,
        11:     opJmpifNp,
        12:     opJmp,
        13:     opReturn,
        14:     opCmpeq,
        15:     opCmplt,
        16:     opCmple,
        17:     opCmpgr,
        18:     opCmpge,
        19:     opCmpne,
        20:     opXor,
        21:     opMod,
        22:     opBitand,
        23:     opBitor,
        24:     opNot,
        25:     opNotf,
        26:     opOnescomplement,
        27:     opShr,
        28:     opShl,
        29:     opAnd,
        30:     opOr,
        31:     opAdd,
        32:     opSub,
        33:     opMul,
        34:     opDiv,
        35:     opNeg,
        36:     opSetcurvar,
        37:     opSetcurvar,
        38:     opSetcurvar,
        39:     opSetcurvar,
        40:     opSetcurvarArray,
        41:     opSetcurvarArray,
        42:     opSetcurvarArray,
        43:     opSetcurvarArray,
        44:     opLoadvarUint,
        45:     opLoadvarFlt,
        46:     opLoadvarStr,
        47:     opSavevarUint,
        48:     opSavevarFlt,
        49:     opSavevarStr,
        50:     opSetcurobject,
        51:     opSetcurobjectNew,
        52:     opSetcurfield,
        53:     opSetcurfieldArray,
        54:     opLoadfieldUint,
        55:     opLoadfieldFlt,
        56:     opLoadfieldStr,
        57:     opSavefieldUint,
        58:     opSavefieldFlt,
        59:     opSavefieldStr,
        60:     opStrToUint,
        61:     opStrToFlt,
        62:     opStrToNone,
        63:     opFltToUint,
        64:     opFltToStr,
        65:     opFltToNone,
        66:     opUintToFlt,
        67:     opUintToStr,
        68:     opUintToNone,
        69:     opLoadimmedUint,
        70:     opLoadimmedFlt,
        71:     opLoadimmedStr,
        72:     opLoadimmedIdent,
        73:     opTagToStr,
        74:     opCallfunc,
        75:     opCallfunc,

        77:     opAdvanceStr,
        78:     opAdvanceStrAppendchar,
        79:     opAdvanceStrComma,
        80:     opAdvanceStrNul,
        81:     opRewindStr,
        82:     opTerminateRewindStr,
        83:     opCompareStr,
        84:     opPush,
        85:     opPushFrame
    }

    '''
    Decodes parsed file
    '''
    def decode(self):
        while self.ip < self.file.byteCode.binLen:
            try:
                # If one or more code block have ended:
                if self.ip in self.endBlock:
                    for end in self.endBlock.pop(self.ip):
                        self.tree.focusParent()
                        if isinstance(end, If) and end.elseHandle is not None:
                            # Append else to tree:
                            self.tree.append(end.elseHandle)
                            self.tree.focusChild()
                        elif isinstance(end, FuncDecl):
                            # Exit function:
                            self.inFunction -= 1

                # Get current opcode:
                opCode = self.getCode()

                # Call its respective routine:
                self.callOp[opCode](self)

                # Record call:
                self.callStack.append(self.callOp[opCode])

                # Update instruction pointer:
                self.ip = self.getCurByteIndex()
            except Exception as e:
                if e.__class__ is KeyError and opCode == self.file.byteCode.endCtrlCode:
                    logging.debug("IP: {}: Got (supposed) end control sequence: Terminating".format(self.ip))
                    return
                else:
                    raise e

if debug:
    logging.basicConfig(level=logging.DEBUG, format="[%(levelname)s]: %(filename)s: %(lineno)d: %(message)s", stream=stdout)
else:
    logging.basicConfig(level=logging.INFO, format="[%(levelname)s]: %(filename)s: %(lineno)d: %(message)s", stream=stdout)

success = []
failed = []

for path in [Path(f) for f in fnames]:
    logging.info("Decompiling file: {}".format(path.name))

    logging.info("Parsing file: {}".format(path.name))
    #try:
    myFile = dsoFile(path)
    myFile.parse()
    #except Exception as e:
    #    print(
    #        type(e).__name__,          # TypeError
    #        __file__,                  # /tmp/example.py
    #        e.__traceback__.tb_lineno  # 2
    #    )
    #    logging.error("Failed to parse file: {}: Got exception: {}".format(path.name, repr(e)))
    #    failed.append(path)
    #    continue

    logging.info("Successfully parsed file: {}".format(path.name))

    if parseOnly:
        outPath = path.with_suffix(path.suffix + ".txt")
        with open(outPath, "w") as fd:
            myFile.dump(sink=fd)

        logging.info("Parse only option is enabled: Output stored in: {}".format(outPath))
        success.append(path)
    else:
        logging.info("Decoding file: {}".format(path.name))
        try:
            decoder = codecDecoding(myFile)
            decoder.decode()
        except Exception as e:
            logging.error("Failed to decode file: {}: Got exception: {}".format(path.name, repr(e)))
            failed.append(path)
            continue

        logging.info("Successfully decoded file: {}".format(path.name))

        decoder.tree.rewind()

        outPath = path.with_suffix(path.suffix + ".cs")

        logging.info("Formatting file: {}".format(path.name))
        try:
            with open(outPath, "w") as fd:
                decoder.tree.format(sink=fd)
        except Exception as e:
            logging.error("Failed to format file: {}: Got exception: {}".format(path.name, repr(e)))
            failed.append(path)

            if outPath.is_file():
                remove(outPath)

            continue
			
        logging.info("Successfully formatted file: {}".format(path.name))

        logging.info("Successfully decompiled file: {}".format(path.name))
        logging.info("Output stored in: {}".format(outPath))

        success.append(path)

if failed:
    logging.info("The following files failed to be decompiled:")

    for path in failed:
        logging.info(str(path))

logging.info("Successfully decompiled {} out of {} input files".format(len(success), len(fnames)))