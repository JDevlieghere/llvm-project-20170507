//===-- Type.cpp ------------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Other libraries and framework includes

#include "lldb/Core/DataExtractor.h"
#include "lldb/Core/DataBufferHeap.h"
#include "lldb/Core/Module.h"
#include "lldb/Core/Scalar.h"
#include "lldb/Core/StreamString.h"

#include "lldb/Symbol/ClangASTType.h"
#include "lldb/Symbol/ClangASTContext.h"
#include "lldb/Symbol/ObjectFile.h"
#include "lldb/Symbol/SymbolContextScope.h"
#include "lldb/Symbol/SymbolFile.h"
#include "lldb/Symbol/Type.h"
#include "lldb/Symbol/TypeList.h"

#include "lldb/Target/ExecutionContext.h"
#include "lldb/Target/Process.h"

using namespace lldb;

lldb_private::Type::Type
(
    lldb::user_id_t uid,
    SymbolFile* symbol_file,
    const ConstString &name,
    uint32_t byte_size,
    SymbolContextScope *context,
    uintptr_t encoding_data,
    EncodingDataType encoding_data_type,
    const Declaration& decl,
    clang_type_t clang_type,
    ResolveState clang_type_resolve_state
) :
    UserID (uid),
    m_name (name),
    m_symbol_file (symbol_file),
    m_context (context),
    m_encoding_type (NULL),
    m_encoding_uid_type (encoding_data_type),
    m_encoding_uid (encoding_data),
    m_byte_size (byte_size),
    m_decl (decl),
    m_clang_type (clang_type),
    m_clang_type_resolve_state (clang_type ? clang_type_resolve_state : eResolveStateUnresolved)
{
}

lldb_private::Type::Type () :
    UserID (0),
    m_name ("<INVALID TYPE>"),
    m_symbol_file (NULL),
    m_context (NULL),
    m_encoding_type (NULL),
    m_encoding_uid_type (eEncodingInvalid),
    m_encoding_uid (0),
    m_byte_size (0),
    m_decl (),
    m_clang_type (NULL),
    m_clang_type_resolve_state (eResolveStateUnresolved)
{
}


const lldb_private::Type&
lldb_private::Type::operator= (const Type& rhs)
{
    if (this != &rhs)
    {
        UserID::operator= (rhs);
        m_name = rhs.m_name;
        m_symbol_file = rhs.m_symbol_file;
        m_context = rhs.m_context;
        m_encoding_type = rhs.m_encoding_type;
        m_encoding_uid_type = rhs.m_encoding_uid_type;
        m_encoding_uid = rhs.m_encoding_uid;
        m_byte_size = rhs.m_byte_size;
        m_decl = rhs.m_decl;
        m_clang_type = rhs.m_clang_type;
        m_clang_type_resolve_state = rhs.m_clang_type_resolve_state;
    }
    return *this;
}


void
lldb_private::Type::GetDescription (Stream *s, lldb::DescriptionLevel level, bool show_name)
{
    *s << "id = " << (const UserID&)*this;

    // Call the name accessor to make sure we resolve the type name
    if (show_name && GetName())
        *s << ", name = \"" << m_name << '"';

    // Call the get byte size accesor so we resolve our byte size
    if (GetByteSize())
        s->Printf(", byte-size = %zu", m_byte_size);
    bool show_fullpaths = (level == lldb::eDescriptionLevelVerbose);
    m_decl.Dump(s, show_fullpaths);

    if (m_clang_type)
    {
        *s << ", clang_type = \"";
        ClangASTType::DumpTypeDescription (GetClangAST(), m_clang_type, s);
        *s << '"';
    }
    else if (m_encoding_uid != LLDB_INVALID_UID)
    {
        s->Printf(", type_uid = 0x%8.8x", m_encoding_uid);
        switch (m_encoding_uid_type)
        {
        case eEncodingIsUID: s->PutCString(" (unresolved type)"); break;
        case eEncodingIsConstUID: s->PutCString(" (unresolved const type)"); break;
        case eEncodingIsRestrictUID: s->PutCString(" (unresolved restrict type)"); break;
        case eEncodingIsVolatileUID: s->PutCString(" (unresolved volatile type)"); break;
        case eEncodingIsTypedefUID: s->PutCString(" (unresolved typedef)"); break;
        case eEncodingIsPointerUID: s->PutCString(" (unresolved pointer)"); break;
        case eEncodingIsLValueReferenceUID: s->PutCString(" (unresolved L value reference)"); break;
        case eEncodingIsRValueReferenceUID: s->PutCString(" (unresolved R value reference)"); break;
        case eEncodingIsSyntheticUID: s->PutCString(" (synthetic type)"); break;
        }
    }    
}


void
lldb_private::Type::Dump (Stream *s, bool show_context)
{
    s->Printf("%.*p: ", (int)sizeof(void*) * 2, this);
    s->Indent();
    *s << "Type" << (const UserID&)*this << ' ';
    if (m_name)
        *s << ", name = \"" << m_name << "\"";

    if (m_byte_size != 0)
        s->Printf(", size = %zu", m_byte_size);

    if (show_context && m_context != NULL)
    {
        s->PutCString(", context = ( ");
        m_context->DumpSymbolContext(s);
        s->PutCString(" )");
    }

    bool show_fullpaths = false;
    m_decl.Dump (s,show_fullpaths);

    if (m_clang_type)
    {
        *s << ", clang_type = " << m_clang_type << ' ';

        ClangASTType::DumpTypeDescription (GetClangAST(), m_clang_type, s);
    }
    else if (m_encoding_uid != LLDB_INVALID_UID)
    {
        *s << ", type_data = " << (uint64_t)m_encoding_uid;
        switch (m_encoding_uid_type)
        {
        case eEncodingIsUID: s->PutCString(" (unresolved type)"); break;
        case eEncodingIsConstUID: s->PutCString(" (unresolved const type)"); break;
        case eEncodingIsRestrictUID: s->PutCString(" (unresolved restrict type)"); break;
        case eEncodingIsVolatileUID: s->PutCString(" (unresolved volatile type)"); break;
        case eEncodingIsTypedefUID: s->PutCString(" (unresolved typedef)"); break;
        case eEncodingIsPointerUID: s->PutCString(" (unresolved pointer)"); break;
        case eEncodingIsLValueReferenceUID: s->PutCString(" (unresolved L value reference)"); break;
        case eEncodingIsRValueReferenceUID: s->PutCString(" (unresolved R value reference)"); break;
        case eEncodingIsSyntheticUID: s->PutCString(" (synthetic type)"); break;
        }
    }

//
//  if (m_access)
//      s->Printf(", access = %u", m_access);
    s->EOL();
}

const lldb_private::ConstString &
lldb_private::Type::GetName()
{
    if (!(m_name))
    {
        if (ResolveClangType(eResolveStateForward))
        {
            std::string type_name = ClangASTContext::GetTypeName (m_clang_type);
            if (!type_name.empty())
                m_name.SetCString (type_name.c_str());
        }
    }
    return m_name;
}

void
lldb_private::Type::DumpTypeName(Stream *s)
{
    GetName().Dump(s, "<invalid-type-name>");
}


void
lldb_private::Type::DumpValue
(
    lldb_private::ExecutionContext *exe_ctx,
    lldb_private::Stream *s,
    const lldb_private::DataExtractor &data,
    uint32_t data_byte_offset,
    bool show_types,
    bool show_summary,
    bool verbose,
    lldb::Format format
)
{
    if (ResolveClangType(eResolveStateForward))
    {
        if (show_types)
        {
            s->PutChar('(');
            if (verbose)
                s->Printf("Type{0x%8.8x} ", GetID());
            DumpTypeName (s);
            s->PutCString(") ");
        }

        lldb_private::ClangASTType::DumpValue (GetClangAST (),
                                               m_clang_type,
                                               exe_ctx,
                                               s,
                                               format == lldb::eFormatDefault ? GetFormat() : format,
                                               data,
                                               data_byte_offset,
                                               GetByteSize(),
                                               0, // Bitfield bit size
                                               0, // Bitfield bit offset
                                               show_types,
                                               show_summary,
                                               verbose,
                                               0);
    }
}

lldb_private::Type *
lldb_private::Type::GetEncodingType ()
{
    if (m_encoding_type == NULL && m_encoding_uid != LLDB_INVALID_UID)
        m_encoding_type = m_symbol_file->ResolveTypeUID(m_encoding_uid);
    return m_encoding_type;
}
    


uint64_t
lldb_private::Type::GetByteSize()
{
    if (m_byte_size == 0)
    {
        switch (m_encoding_uid_type)
        {
        case eEncodingIsUID:
        case eEncodingIsConstUID:
        case eEncodingIsRestrictUID:
        case eEncodingIsVolatileUID:
        case eEncodingIsTypedefUID:
            {
                Type *encoding_type = GetEncodingType ();
                if (encoding_type)
                    m_byte_size = encoding_type->GetByteSize();
                if (m_byte_size == 0)
                {
                    uint64_t bit_width = ClangASTType::GetClangTypeBitWidth (GetClangAST(), GetClangLayoutType());
                    m_byte_size = (bit_width + 7 ) / 8;
                }
            }
            break;

        // If we are a pointer or reference, then this is just a pointer size;
        case eEncodingIsPointerUID:
        case eEncodingIsLValueReferenceUID:
        case eEncodingIsRValueReferenceUID:
            m_byte_size = GetTypeList()->GetClangASTContext().GetPointerBitSize() / 8;
            break;
        }
    }
    return m_byte_size;
}


uint32_t
lldb_private::Type::GetNumChildren (bool omit_empty_base_classes)
{
    if (!ResolveClangType(eResolveStateFull))
        return 0;
    return ClangASTContext::GetNumChildren (m_clang_type, omit_empty_base_classes);

}

bool
lldb_private::Type::IsAggregateType ()
{
    if (ResolveClangType(eResolveStateForward))
        return ClangASTContext::IsAggregateType (m_clang_type);
    return false;
}

lldb::Format
lldb_private::Type::GetFormat ()
{
    // Make sure we resolve our type if it already hasn't been.
    if (!ResolveClangType(eResolveStateForward))
        return lldb::eFormatInvalid;
    return lldb_private::ClangASTType::GetFormat (m_clang_type);
}



lldb::Encoding
lldb_private::Type::GetEncoding (uint32_t &count)
{
    // Make sure we resolve our type if it already hasn't been.
    if (!ResolveClangType(eResolveStateForward))
        return lldb::eEncodingInvalid;

    return lldb_private::ClangASTType::GetEncoding (m_clang_type, count);
}



bool
lldb_private::Type::DumpValueInMemory
(
    lldb_private::ExecutionContext *exe_ctx,
    lldb_private::Stream *s,
    lldb::addr_t address,
    lldb::AddressType address_type,
    bool show_types,
    bool show_summary,
    bool verbose
)
{
    if (address != LLDB_INVALID_ADDRESS)
    {
        lldb_private::DataExtractor data;
        data.SetByteOrder (exe_ctx->process->GetByteOrder());
        if (ReadFromMemory (exe_ctx, address, address_type, data))
        {
            DumpValue(exe_ctx, s, data, 0, show_types, show_summary, verbose);
            return true;
        }
    }
    return false;
}


bool
lldb_private::Type::ReadFromMemory (lldb_private::ExecutionContext *exe_ctx, lldb::addr_t addr, lldb::AddressType address_type, lldb_private::DataExtractor &data)
{
    if (address_type == lldb::eAddressTypeFile)
    {
        // Can't convert a file address to anything valid without more
        // context (which Module it came from)
        return false;
    }

    const uint32_t byte_size = GetByteSize();
    if (data.GetByteSize() < byte_size)
    {
        lldb::DataBufferSP data_sp(new DataBufferHeap (byte_size, '\0'));
        data.SetData(data_sp);
    }

    uint8_t* dst = (uint8_t*)data.PeekData(0, byte_size);
    if (dst != NULL)
    {
        if (address_type == lldb::eAddressTypeHost)
        {
            // The address is an address in this process, so just copy it
            memcpy (dst, (uint8_t*)NULL + addr, byte_size);
            return true;
        }
        else
        {
            if (exe_ctx && exe_ctx->process)
            {
                Error error;
                return exe_ctx->process->ReadMemory(addr, dst, byte_size, error) == byte_size;
            }
        }
    }
    return false;
}


bool
lldb_private::Type::WriteToMemory (lldb_private::ExecutionContext *exe_ctx, lldb::addr_t addr, lldb::AddressType address_type, lldb_private::DataExtractor &data)
{
    return false;
}


lldb_private::TypeList*
lldb_private::Type::GetTypeList()
{
    return GetSymbolFile()->GetTypeList();
}

const lldb_private::Declaration &
lldb_private::Type::GetDeclaration () const
{
    return m_decl;
}

bool
lldb_private::Type::ResolveClangType (ResolveState clang_type_resolve_state)
{
    Type *encoding_type = NULL;
    if (m_clang_type == NULL)
    {
        TypeList *type_list = GetTypeList();
        encoding_type = GetEncodingType();
        if (encoding_type)
        {
            switch (m_encoding_uid_type)
            {
            case eEncodingIsUID:
                if (encoding_type->ResolveClangType(clang_type_resolve_state))
                {
                    m_clang_type = encoding_type->m_clang_type;
                    m_clang_type_resolve_state = encoding_type->m_clang_type_resolve_state;
                }
                break;

            case eEncodingIsConstUID:
                m_clang_type = ClangASTContext::AddConstModifier (encoding_type->GetClangForwardType());
                break;

            case eEncodingIsRestrictUID:
                m_clang_type = ClangASTContext::AddRestrictModifier (encoding_type->GetClangForwardType());
                break;

            case eEncodingIsVolatileUID:
                m_clang_type = ClangASTContext::AddVolatileModifier (encoding_type->GetClangForwardType());
                break;

            case eEncodingIsTypedefUID:
                m_clang_type = type_list->CreateClangTypedefType (this, encoding_type);
                // Clear the name so it can get fully qualified in case the
                // typedef is in a namespace.
                m_name.Clear();
                break;

            case eEncodingIsPointerUID:
                m_clang_type = type_list->CreateClangPointerType (encoding_type);
                break;

            case eEncodingIsLValueReferenceUID:
                m_clang_type = type_list->CreateClangLValueReferenceType (encoding_type);
                break;

            case eEncodingIsRValueReferenceUID:
                m_clang_type = type_list->CreateClangRValueReferenceType (encoding_type);
                break;

            default:
                assert(!"Unhandled encoding_data_type.");
                break;
            }
        }
        else
        {
            // We have no encoding type, return void?
            clang_type_t void_clang_type = type_list->GetClangASTContext().GetBuiltInType_void();
            switch (m_encoding_uid_type)
            {
            case eEncodingIsUID:
                m_clang_type = void_clang_type;
                break;

            case eEncodingIsConstUID:
                m_clang_type = ClangASTContext::AddConstModifier (void_clang_type);
                break;

            case eEncodingIsRestrictUID:
                m_clang_type = ClangASTContext::AddRestrictModifier (void_clang_type);
                break;

            case eEncodingIsVolatileUID:
                m_clang_type = ClangASTContext::AddVolatileModifier (void_clang_type);
                break;

            case eEncodingIsTypedefUID:
                m_clang_type = type_list->GetClangASTContext().CreateTypedefType (m_name.AsCString(), void_clang_type, NULL);
                break;

            case eEncodingIsPointerUID:
                m_clang_type = type_list->GetClangASTContext().CreatePointerType (void_clang_type);
                break;

            case eEncodingIsLValueReferenceUID:
                m_clang_type = type_list->GetClangASTContext().CreateLValueReferenceType (void_clang_type);
                break;

            case eEncodingIsRValueReferenceUID:
                m_clang_type = type_list->GetClangASTContext().CreateRValueReferenceType (void_clang_type);
                break;

            default:
                assert(!"Unhandled encoding_data_type.");
                break;
            }
        }
    }
    
    // Check if we have a forward reference to a class/struct/union/enum?
    if (m_clang_type && m_clang_type_resolve_state < clang_type_resolve_state)
    {
        m_clang_type_resolve_state = eResolveStateFull;
        if (!ClangASTType::IsDefined (m_clang_type))
        {
            // We have a forward declaration, we need to resolve it to a complete
            // definition.
            m_symbol_file->ResolveClangOpaqueTypeDefinition (m_clang_type);
        }
    }
    
    // If we have an encoding type, then we need to make sure it is 
    // resolved appropriately.
    if (m_encoding_uid != LLDB_INVALID_UID)
    {
        if (encoding_type == NULL)
            encoding_type = GetEncodingType();
        if (encoding_type)
        {
            ResolveState encoding_clang_type_resolve_state = eResolveStateFull;
            switch (m_encoding_uid_type)
            {
            case eEncodingIsPointerUID:
            case eEncodingIsLValueReferenceUID:
            case eEncodingIsRValueReferenceUID:
                if (clang_type_resolve_state == eResolveStateLayout)
                    encoding_clang_type_resolve_state = eResolveStateForward;
                break;
            default:
                break;
            }
            encoding_type->ResolveClangType (encoding_clang_type_resolve_state);
        }
    }
    return m_clang_type != NULL;
}

clang_type_t 
lldb_private::Type::GetChildClangTypeAtIndex
(
    const char *parent_name,
    uint32_t idx,
    bool transparent_pointers,
    bool omit_empty_base_classes,
    ConstString& name,
    uint32_t &child_byte_size,
    int32_t &child_byte_offset,
    uint32_t &child_bitfield_bit_size,
    uint32_t &child_bitfield_bit_offset,
    bool &child_is_base_class
)
{
    clang_type_t child_qual_type = NULL;
    
    if (GetClangType())
    {
        std::string name_str;
        child_qual_type = GetClangASTContext().GetChildClangTypeAtIndex (parent_name,
                                                                         m_clang_type,
                                                                         idx,
                                                                         transparent_pointers,
                                                                         omit_empty_base_classes,
                                                                         name_str,
                                                                         child_byte_size,
                                                                         child_byte_offset,
                                                                         child_bitfield_bit_size,
                                                                         child_bitfield_bit_offset,
                                                                         child_is_base_class);
        
        if (child_qual_type)
        {
            if (!name_str.empty())
                name.SetCString(name_str.c_str());
            else
                name.Clear();
        }
    }
    return child_qual_type;
}


clang_type_t 
lldb_private::Type::GetClangType ()
{
    ResolveClangType(eResolveStateFull);
    return m_clang_type;
}

clang_type_t 
lldb_private::Type::GetClangLayoutType ()
{
    ResolveClangType(eResolveStateLayout);
    return m_clang_type;
}

clang_type_t 
lldb_private::Type::GetClangForwardType ()
{
    ResolveClangType (eResolveStateForward);
    return m_clang_type;
}

clang::ASTContext *
lldb_private::Type::GetClangAST ()
{
    TypeList *type_list = GetTypeList();
    if (type_list)
        return type_list->GetClangASTContext().getASTContext();
    return NULL;
}

lldb_private::ClangASTContext &
lldb_private::Type::GetClangASTContext ()
{
    return GetTypeList()->GetClangASTContext();
}

int
lldb_private::Type::Compare(const Type &a, const Type &b)
{
    // Just compare the UID values for now...
    lldb::user_id_t a_uid = a.GetID();
    lldb::user_id_t b_uid = b.GetID();
    if (a_uid < b_uid)
        return -1;
    if (a_uid > b_uid)
        return 1;
    return 0;
//  if (a.getQualType() == b.getQualType())
//      return 0;
}

