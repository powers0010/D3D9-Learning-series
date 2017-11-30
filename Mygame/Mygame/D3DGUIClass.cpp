#include "StdAfx.h"
#include "D3DGUIClass.h"

D3DGUIClass::D3DGUIClass(LPDIRECT3DDEVICE9 device, int w, int h)
{
	m_pd3dDevice=device;
	m_pVertexBuffer=NULL;
	m_pControls=NULL;
	m_BackgroundBuffer=NULL;
	m_pFonts=NULL;
	m_bIsBackgroundUsed=false;
	m_nTotalFontNum=0;
	m_nTotalControlNum=0;
	m_nTotalBufferNum=0;
	m_nWindowWidth=w;
	m_nWindowHeight=h;
}
bool D3DGUIClass::CreateTextFont(wchar_t *fontName, int size, int *fontID)  //���崴������
{
	return true;
}
bool D3DGUIClass::AddBackground(wchar_t *fileName)  //GUI������Ӻ���
{
	// 	GUICONTROL					m_Background;		//����ͼ����
	// 	IDirect3DVertexBuffer9	*	m_BackgroundBuffer;	//����ͼ����������
	if (!fileName)
	{
		return false;
	}
	m_Background.m_type=UGP_GUI_Background;

	//��������
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,fileName,&m_Background.m_Background)))
	{
		return false;
	}

	// ��ȡ���ڿ�Ⱥ͸߶ȣ��Ա��Ժ�ı�����������
	float w = (float)m_nWindowWidth;
	float h = (float)m_nWindowHeight;

	GUIVERTEX obj[] =
	{
		{w, 0, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f},
		{w, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f},
		{0, 0, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f},
		{0, h, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f},
	};
	//�������㻺��
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(sizeof(obj),NULL,D3DFVF_GUI, D3DPOOL_MANAGED,&m_BackgroundBuffer,NULL)))
	{
		return false;
	}
	void *ptr;
	if (FAILED(m_BackgroundBuffer->Lock(0,sizeof(obj),&ptr,0)))	//����
	{
		return false;
	}
	memcpy(ptr,obj,sizeof(obj));	//��䶥�㻺����
	m_BackgroundBuffer->Unlock();	//����

	m_bIsBackgroundUsed = true;		//����״̬��Ϊtrue

	return true;


}
bool D3DGUIClass::AddStaticText(int id, wchar_t *text, float x, float y, unsigned long color, int fontID) //��Ӿ�̬�ı�����
{
	return true;
}
bool D3DGUIClass::AddButton(int id, float x, float y, wchar_t *up, wchar_t *over, wchar_t *down) //��Ӱ�ť����
{
	if(!up || !over || !down) return false;
	if (!m_pControls)
	{
		m_pControls	=	new GUICONTROL [1];
		if(!m_pControls) return false;
		memset(&m_pControls[0], 0, sizeof(GUICONTROL));
	}
	else
	{
		GUICONTROL *temp=new GUICONTROL[m_nTotalControlNum+1];
		if(!temp) return false;
		memset(temp, 0, sizeof(GUICONTROL) * (m_nTotalControlNum + 1));
		memcpy(temp, m_pControls,sizeof(GUICONTROL) * m_nTotalControlNum);
		delete [] m_pControls;
		m_pControls=temp;
	}

	m_pControls[m_nTotalControlNum].m_type=UGP_GUI_BUTTON;
	m_pControls[m_nTotalControlNum].m_id=id;
	m_pControls[m_nTotalControlNum].m_xPos=x;
	m_pControls[m_nTotalControlNum].m_yPos=y;
	m_pControls[m_nTotalControlNum].m_listID=m_nTotalControlNum;

	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,up,&m_pControls[m_nTotalControlNum].m_upTex)))
	{
		return false;
	}
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,over,&m_pControls[m_nTotalControlNum].m_overTex)))
	{
		return false;
	}
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice,down,&m_pControls[m_nTotalControlNum].m_downTex)))
	{
		return false;
	}
	//��ȡͼƬ�ߴ�
	D3DSURFACE_DESC suf;
	m_pControls[m_nTotalControlNum].m_upTex->GetLevelDesc(0,&suf);

	float w =(float)suf.Width;
	float h= (float)suf.Height;
	m_pControls[m_nTotalControlNum].m_width	=	w;
	m_pControls[m_nTotalControlNum].m_height=	h;
	//����
	GUIVERTEX obj[]=
	{
		{w + x, 0 + y, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f},
		{w + x, h + y, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f},
		{0 + x, 0 + y, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f},
		{0 + x, h + y, 0.0f, 1, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f},
	};
	//�������㻺��
	if( !m_pVertexBuffer)
	{
		m_pVertexBuffer = new IDirect3DVertexBuffer9 * [1];
		if(!m_pVertexBuffer) return false;
	}
	else
	{
		IDirect3DVertexBuffer9** temp = new IDirect3DVertexBuffer9 * [m_nTotalControlNum+1];
		if(!temp) return false;
		memcpy(temp, m_pVertexBuffer,sizeof(LPDIRECT3DVERTEXBUFFER9) * m_nTotalBufferNum);

		delete[] m_pVertexBuffer;
		m_pVertexBuffer = temp;
	}
	if(FAILED(m_pd3dDevice->CreateVertexBuffer(sizeof(obj), 0,
		D3DFVF_GUI, D3DPOOL_DEFAULT, &m_pVertexBuffer[m_nTotalBufferNum], NULL)))
	{
		return false;
	}
	//��䶥��
	void * ptr;
	if (FAILED(m_pVertexBuffer[m_nTotalControlNum]->Lock(0,sizeof(obj),(void**)&ptr,0)))
	{
		return false;
	}
	memcpy(ptr, obj, sizeof(obj));
	m_pVertexBuffer[m_nTotalBufferNum]->Unlock();

	// ����
	m_nTotalBufferNum++;

	// ����
	m_nTotalControlNum++;

	return true;

}
void D3DGUIClass::ClearUp( ) //��Դ������
{
	//�����Ż����ͷ����е���Դ
	if(m_bIsBackgroundUsed)
	{
		if(m_Background.m_Background)
			m_Background.m_Background->Release();

		if(m_BackgroundBuffer)
			m_BackgroundBuffer->Release();
	}

	m_Background.m_Background = NULL;
	m_BackgroundBuffer = NULL;

	for(int i = 0; i < m_nTotalFontNum; i++)
	{
		if(m_pFonts[i])
		{
			m_pFonts[i]->Release();
			m_pFonts[i] = NULL;
		}
	}

	if(m_pFonts) delete[] m_pFonts;
	m_pFonts = NULL;
	m_nTotalFontNum = 0;

	for(int i = 0; i < m_nTotalBufferNum; i++)
	{
		if(m_pVertexBuffer[i])
		{
			m_pVertexBuffer[i]->Release();
			m_pVertexBuffer[i] = NULL;
		}
	}

	if(m_pVertexBuffer) delete[] m_pVertexBuffer;
	m_pVertexBuffer = NULL;
	m_nTotalBufferNum = 0;

	for(int i = 0; i < m_nTotalControlNum; i++)
	{
		if(m_pControls[i].m_Background)
		{
			m_pControls[i].m_Background->Release();
			m_pControls[i].m_Background = NULL;
		}

		if(m_pControls[i].m_upTex)
		{
			m_pControls[i].m_upTex->Release();
			m_pControls[i].m_upTex = NULL;
		}

		if(m_pControls[i].m_downTex)
		{
			m_pControls[i].m_downTex->Release();
			m_pControls[i].m_downTex = NULL;
		}

		if(m_pControls[i].m_overTex)
		{
			m_pControls[i].m_overTex->Release();
			m_pControls[i].m_overTex = NULL;
		}

		if(m_pControls[i].m_text)
		{
			delete[] m_pControls[i].m_text;
			m_pControls[i].m_text = NULL;
		}
	}

	if(m_pControls) delete[] m_pControls;
	m_pControls = NULL;
	m_nTotalControlNum = 0;
}


void ProcessGUI(D3DGUIClass *gui, bool LMBDown, float mouseX, float mouseY,	void(*funcPtr)(int id, int state))
{
	if (!gui)
	{
		return;
	}
	//��ȡD3D�豸
	LPDIRECT3DDEVICE9 device=gui->GetD3dDevice();
	if (!gui)
	{
		return;
	}

	//���Ʊ���
	GUICONTROL * Background=gui->GetBackground();
	LPDIRECT3DVERTEXBUFFER9 bdBuffer=gui->GetBackgroundBuffer();
	//����
	if (gui->IsBackgroundUsed()&&Background&&bdBuffer)
	{
		device->SetStreamSource(0,bdBuffer,0,sizeof(GUIVERTEX));
		device->SetFVF(D3DFVF_GUI);
		device->SetTexture(0,Background->m_Background);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
		device->SetTexture(0,NULL);
	}

	// ����һ�����㻺��������ڰ�ť����Ⱦ
	LPDIRECT3DVERTEXBUFFER9 pBuffer = NULL;
	int status = UGP_BUTTON_UP;

	// һ��ѭ�������ڸ��ֿؼ�����Ⱦ
	for (int i=0;i<gui->GetTotalControlNum();i++)
	{
		GUICONTROL *pControl = gui->GetGUIControl(i);
		if(!pControl) continue;

		switch(pControl->m_type)
		{
		case UGP_GUI_BUTTON:
			status = UGP_BUTTON_UP;
			//��ȡ��ť����Ӧ�Ķ��㻺�����
			pBuffer = gui->GetVertexBuffer(pControl->m_listID);
			if(!pBuffer) continue;

			// ���������alpha͸��ѡ��
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//�������Ƿ���ͣ���ߵ���˰�ť
			if((mouseX > pControl->m_xPos) && (mouseX < (pControl->m_xPos + pControl->m_width ))&&
				(mouseY > pControl->m_yPos-20) && (mouseY < (pControl->m_yPos + pControl->m_height-20)))
			{
				if(LMBDown)
					status = UGP_BUTTON_DOWN;	
				else 
					status = UGP_BUTTON_OVER;
			}
			
			//���ݲ�ͬ�����Ͱ�ť֮����������״̬��׼����ͬ������ͼ 
			if(status == UGP_BUTTON_UP) device->SetTexture(0, pControl->m_upTex);
			if(status == UGP_BUTTON_OVER) device->SetTexture(0, pControl->m_overTex);
			if(status == UGP_BUTTON_DOWN) device->SetTexture(0, pControl->m_downTex);
			// ���¾㱸����ʼ��Ⱦ��ť
			device->SetStreamSource(0, pBuffer, 0, sizeof(GUIVERTEX));
			device->SetFVF(D3DFVF_GUI);
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// �ر�alpha���.
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;

		}
		funcPtr(pControl->m_id	, status);
	}

}