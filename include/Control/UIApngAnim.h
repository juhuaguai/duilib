#ifndef ApngAnimUI_h__
#define ApngAnimUI_h__

#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	typedef struct t_stbi_apng_frame
	{
		t_stbi_apng_frame()
		{
			width=0;
			height=0;
			x_offset=0;
			y_offset=0;
			delay_num=0;
			delay_den=0;
			dispose_op=0;
			blend_op=0;
			pad[0]=0;
			pad[1]=0;
		};
		t_stbi_apng_frame& operator=(const t_stbi_apng_frame& value)
		{
			width = value.width;
			height = value.height;
			x_offset = value.x_offset;
			y_offset = value.y_offset;
			delay_num = value.delay_num;
			delay_den = value.delay_den;
			dispose_op = value.dispose_op;
			blend_op = value.blend_op;
			pad[0]=value.pad[0];
			pad[1]=value.pad[1];

			return *this;
		};
		UINT32 width;
		UINT32 height;
		UINT32 x_offset;
		UINT32 y_offset;
		UINT16 delay_num;
		UINT16 delay_den;
		UCHAR dispose_op;
		UCHAR blend_op;
		UCHAR pad[2];
	} stbi_apng_frame;

	typedef struct tagApngFrame
	{
		tagApngFrame& operator=(const tagApngFrame& value)
		{
			fram = value.fram;
			stbFram = value.stbFram;

			return *this;
		};

		Frame fram;
		stbi_apng_frame stbFram;
	}ApngFrame;

	class DUILIB_API CApngAnimUI : public CGifAnimUI
	{
	public:
		CApngAnimUI(void);
		~CApngAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);

		void	SetBkImage(const CDuiString& strImage);

		virtual void PaintBkImage(HDC hDC);

		void	Play();

	protected:
		void    OnTimer( UINT_PTR idEvent );
		bool	LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask );
		void	BlendOver(COLORREF* dst, COLORREF* src,UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height);
		void	BlendSource(COLORREF* dst, COLORREF* src,UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height);
		void	BlendZero(COLORREF* dst, UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height);
		HBITMAP CreateFramBmp(LPBYTE pBits,UINT32 dstWidth, UINT32 dstHeight,DWORD dwMask);
	};
}

#endif // ApngAnimUI_h__
