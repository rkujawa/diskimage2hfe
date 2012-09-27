/*
//
// Copyright (C) 2006, 2007, 2008, 2009 Jean-François DEL NERO
//
// This file is part of HxCFloppyEmulator.
//
// HxCFloppyEmulator may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/
#include "../common/plugins/adf_loader/adf_loader.h"
#include "../common/plugins/adz_loader/adz_loader.h"
#include "../common/plugins/dms_loader/dms_loader.h"
#include "../common/plugins/amigadosfs_loader/amigadosfs_loader.h"
#include "../common/plugins/ipf_loader/ipf_loader.h"
#include "../common/plugins/hfe_loader/hfe_loader.h"

const plugins_ptr staticplugins[]=
{
	{(ISVALIDDISKFILE)DMS_libIsValidDiskFile,(LOADDISKFILE)DMS_libLoad_DiskFile},
	{(ISVALIDDISKFILE)ADF_libIsValidDiskFile,(LOADDISKFILE)ADF_libLoad_DiskFile},
/*	{(ISVALIDDISKFILE)ADZ_libIsValidDiskFile,(LOADDISKFILE)ADZ_libLoad_DiskFile},
	{(ISVALIDDISKFILE)IPF_libIsValidDiskFile,(LOADDISKFILE)IPF_libLoad_DiskFile},
	{(ISVALIDDISKFILE)AMIGADOSFSDK_libIsValidDiskFile,(LOADDISKFILE)AMIGADOSFSDK_libLoad_DiskFile},
	{(ISVALIDDISKFILE)IMG_libIsValidDiskFile,(LOADDISKFILE)IMG_libLoad_DiskFile},
	{(ISVALIDDISKFILE)HFE_libIsValidDiskFile,(LOADDISKFILE)HFE_libLoad_DiskFile},*/
	{(ISVALIDDISKFILE)-1,(LOADDISKFILE)-1}
};
