#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(AttachmentType, rendergraph::node::AttachRT::AttachmentType, attachment,   m_attachment,   (rendergraph::node::AttachRT::AttachmentType::Color0))
PARAM_INFO(TextureTarget,  rendergraph::node::AttachRT::TextureTarget,  textarget,    m_textarget,    (rendergraph::node::AttachRT::TextureTarget::Texture2D))
PARAM_INFO(MipmapLevel,    size_t,                                      mipmap_level, m_mipmap_level, (0))
