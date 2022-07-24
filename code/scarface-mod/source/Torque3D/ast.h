
/// Representation of a node for the scripting language parser.
///
/// When the scripting language is evaluated, it is turned from a string representation,
/// into a parse tree, thence into byte code, which is ultimately interpreted by the VM.
///
/// This is the base class for the nodes in the parse tree. There are a great many subclasses,
/// each representing a different language construct.
struct StmtNode
{
   StmtNode *next;   ///< Next entry in parse tree.

   StmtNode();
   virtual ~StmtNode() {}
   
   /// @name next Accessors
   /// @{

   ///
   void* unk0;
   StmtNode *getNext() const { return next; }

   /// @}

   /// @name Debug Info
   /// @{

   StringTableEntry dbgFileName; ///< Name of file this node is associated with.
   S32 dbgLineNumber;            ///< Line number this node is associated with.

   /// @}

   /// @name Breaking
   /// @{

   void* unk1;
   void* unk2;
   /// @}

   /// @name Compilation
   /// @{

   U32* unk3;
   U32* unk4;
   void* unk5;
   /// @}
};