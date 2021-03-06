/*#include "Precompile.h"*/
#include "RenderMesh.h"

#include "Foundation/Checksum/CRC32.h"

using namespace Helium;
using namespace Helium::Core;

/////////////////////////////////////////////////////////////////////////////////////////////
Render::Mesh::Mesh(const tchar* name)
{
  m_filename = name;
  m_crc = Helium::StringCrc32(name);
  m_load_count = 0;
  m_timestamp = (u64)-1;
  m_verts = 0;
  m_indices = 0;
  m_vert_count = 0;
  m_index_count = 0;
  m_fragment_count= 0;
  m_fragments = 0;
  m_dbg_normal = 0;
  m_dbg_tangent = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
Render::Mesh::~Mesh()
{
  if (m_fragments)
  {
    delete[] m_fragments;
    m_fragments=0;
  }

  if (m_dbg_normal)
  {
    m_dbg_normal->Release();
    m_dbg_normal = 0;
  }

  if (m_dbg_tangent)
  {
    m_dbg_tangent->Release();
    m_dbg_tangent = 0;
  }

  if (m_verts)
  {
    m_verts->Release();
    m_verts = 0;
  }

  if (m_indices)
  {
    m_indices->Release();
    m_indices = 0;
  }
}

