#ifndef __INT_EEPROM_H__
#define __INT_EEPROM_H__
#include "Com_Util.h"

/**
 * @brief ���Ͷ���ֽ�
 *
 * @param addr �ֽڵ�ַ
 * @param bytes �����͵�����
 * @param len ���鳤��
 */
void Int_EEPROM_WriteBytes(u8 addr, u8 bytes[], u8 len);

/**
 * @brief ���ն���ֽ�
 *
 * @param addr �ֽڵ�ַ
 * @param bytes ���ڽ��ս��������
 * @param len Ҫ��ȡ�ĳ���
 */
void Int_EEPROM_ReadBytes(u8 addr, u8 bytes[], u8 len);

#endif /* __INT_EEPROM_H__ */