#include "common.h"
#include "ZCompressor.h"
#include "WorldSession.h"
#include "Object.h"
#include "UpdateData.h"
#include "UpdateFields.h"
#include "UpdateMask.h"


void WorldSession::_HandleCompressedUpdateObjectOpcode(WorldPacket& recvPacket)
{
    //printf("-> COMPRESSED_UPDATE_OBJECT, pktlen=%u\n",recvPacket.size());
    //recvPacket.hexlike();
    uint32 realsize;
    recvPacket >> realsize;
    ZCompressor z;
    z.append(recvPacket.contents() + sizeof(uint32),recvPacket.size() - sizeof(uint32));
    z.Compressed(true);
    z.RealSize(realsize);
    z.Inflate();
    if(z.Compressed())
    {
        logerror("_HandleCompressedUpdateObjectOpcode(): Inflate() failed!");
        return;
    }
    WorldPacket wp;
    wp.SetOpcode(recvPacket.GetOpcode());
    wp.append(z.contents(),z.size());
 
    _HandleUpdateObjectOpcode(wp);
}

void WorldSession::_HandleUpdateObjectOpcode(WorldPacket& recvPacket)
{
     //recvPacket.hexlike();
     uint8 utype;
     uint8 unk8;
     uint32 usize, ublocks;
     uint64 uguid;
     recvPacket >> ublocks >> unk8;
     logdebug("UpdateObject: ublocks=%u unk=%u",ublocks,unk8);
     //while(true) // need to read full packet as soon as the structure is 100% known & implemented
                   // for now reading first object is enough
     {
         recvPacket >> utype;
         logdebug("UpdateObject: utype=%u",utype);
         switch(utype)
         {
            case UPDATETYPE_OUT_OF_RANGE_OBJECTS:
                recvPacket >> usize;
                for(uint16 i=0;i<usize;i++)
                {
                    uguid = recvPacket.GetPackedGuid(); // not 100% sure if this is correct
                    logdebug("GUID "I64FMT" out of range",uguid);
                    // TODO: delete object from known objects list
                }
            break;

            case UPDATETYPE_VALUES:
            {
                uguid = recvPacket.GetPackedGuid(); // not 100% sure if this is correct
                uint8 maskblocks;
                recvPacket >> maskblocks;
                logdebug("UPDATETYPE_VALUES: guid="I64FMT" maskblocks=%u",uguid,maskblocks);
                // need some help with the lines below, got no idea what to do now.
                UpdateMask umask;
                umask.SetCount(maskblocks*8); // ??
                std::vector<uint8> udata;
                //udata.resize(?)


            }
            break;

            case UPDATETYPE_CREATE_OBJECT:
            case UPDATETYPE_CREATE_OBJECT2:
            {
                uguid = recvPacket.GetPackedGuid();
                uint8 objtypeid;
                recvPacket >> objtypeid;
                logdebug("Create Object type %u with guid "I64FMT,objtypeid,uguid);
                if(objtypeid==TYPEID_PLAYER)
                {

                }
                if(objtypeid==TYPEID_UNIT)
                {

                }
                if( (objtypeid==TYPEID_CORPSE) || (objtypeid==TYPEID_GAMEOBJECT) || (objtypeid==TYPEID_DYNAMICOBJECT))
                {

                }

                // (TODO) and then: Add object to objmgr
            }
            break;


            default:
                break;
         }
     }
}