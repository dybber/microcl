// --------------------------------
// Information about a device
// --------------------------------
typedef struct
{
  char                          m_deviceName[MCL_MAX_STRING_LENGTH];
  char                          m_deviceVendor[MCL_MAX_STRING_LENGTH];
  char                          m_driverVersion[MCL_MAX_STRING_LENGTH];
  char                          m_deviceExtensions[MCL_MAX_STRING_LENGTH];
  cl_device_type                m_deviceType;
  cl_uint                       m_computeUnits;
  size_t                        m_workitemDims;
  size_t                        m_workItemSize[3];
  size_t                        m_image2dMaxWidth;
  size_t                        m_image2dMaxHeight;
  size_t                        m_image3dMaxWidth;
  size_t                        m_image3dMaxHeight;
  size_t                        m_image3dMaxDepth;
  size_t                        m_workgroupSize;
  cl_uint                       m_clockFrequency;
  cl_ulong                      m_constantBufferSize;
  cl_ulong                      m_localMemSize;
  cl_ulong                      m_globalMemSize;
  cl_bool                       m_errorCorrectionSupport;
  cl_device_local_mem_type      m_localMemType;
  cl_uint                       m_maxReadImageArgs;
  cl_uint                       m_maxWriteImageArgs;
  cl_uint                       m_addressBits;
  cl_ulong                      m_maxMemAllocSize;
  cl_command_queue_properties   m_queueProperties;
  cl_bool                       m_imageSupport;
  cl_uint                       m_vecWidthChar;
  cl_uint                       m_vecWidthShort;
  cl_uint                       m_vecWidthInt;
  cl_uint                       m_vecWidthLong;
  cl_uint                       m_vecWidthFloat;
  cl_uint                       m_vecWidthDouble;
  size_t                        m_profilingTimerResolution;
} mclDeviceInfo;

void mclGetDeviceInfo(cl_device_id device, mclDeviceInfo* info) {
  // CL_DEVICE_NAME
  clGetDeviceInfo(device, CL_DEVICE_NAME, MCL_MAX_STRING_LENGTH, &info->m_deviceName, NULL);
  // CL_DEVICE_VENDOR
  clGetDeviceInfo(device, CL_DEVICE_VENDOR, MCL_MAX_STRING_LENGTH, &info->m_deviceVendor, NULL);
  // CL_DRIVER_VERSION
  clGetDeviceInfo(device, CL_DRIVER_VERSION, MCL_MAX_STRING_LENGTH, &info->m_driverVersion, NULL);
  // CL_DEVICE_INFO
  clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &info->m_deviceType, NULL);
  // CL_DEVICE_MAX_COMPUTE_UNITS
  clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(info->m_computeUnits), &info->m_computeUnits, NULL);
  // CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
  clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(info->m_workitemDims), &info->m_workitemDims, NULL);
  // CL_DEVICE_MAX_WORK_ITEM_SIZES
  clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(info->m_workItemSize), &info->m_workItemSize, NULL);
  // CL_DEVICE_MAX_WORK_GROUP_SIZE
  clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(info->m_workgroupSize), &info->m_workgroupSize, NULL);
  // CL_DEVICE_MAX_CLOCK_FREQUENCY
  clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(info->m_clockFrequency), &info->m_clockFrequency, NULL);
  // CL_DEVICE_ADDRESS_BITS
  clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(info->m_addressBits), &info->m_addressBits, NULL);
  // CL_DEVICE_MAX_MEM_ALLOC_SIZE
  clGetDeviceInfo(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(info->m_maxMemAllocSize), &info->m_maxMemAllocSize, NULL);
  // CL_DEVICE_GLOBAL_MEM_SIZE
  clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(info->m_globalMemSize), &info->m_globalMemSize, NULL);
  // CL_DEVICE_ERROR_CORRECTION_SUPPORT
  clGetDeviceInfo(device, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(info->m_errorCorrectionSupport), &info->m_errorCorrectionSupport, NULL);
  // CL_DEVICE_LOCAL_MEM_TYPE
  clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(info->m_localMemType), &info->m_localMemType, NULL);
  // CL_DEVICE_LOCAL_MEM_SIZE
  clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(info->m_localMemSize), &info->m_localMemSize, NULL);
  // CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
  clGetDeviceInfo(device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(info->m_constantBufferSize), &info->m_constantBufferSize, NULL);
  // CL_DEVICE_QUEUE_PROPERTIES
  clGetDeviceInfo(device, CL_DEVICE_QUEUE_PROPERTIES, sizeof(info->m_queueProperties), &info->m_queueProperties, NULL);
  // CL_DEVICE_IMAGE_SUPPORT
  clGetDeviceInfo(device, CL_DEVICE_IMAGE_SUPPORT, sizeof(info->m_imageSupport), &info->m_imageSupport, NULL);
  // CL_DEVICE_MAX_READ_IMAGE_ARGS
  clGetDeviceInfo(device, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(info->m_maxReadImageArgs), &info->m_maxReadImageArgs, NULL);
  // CL_DEVICE_MAX_WRITE_IMAGE_ARGS
  clGetDeviceInfo(device, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(info->m_maxWriteImageArgs), &info->m_maxWriteImageArgs, NULL);
  // CL_DEVICE_IMAGE2D_MAX_WIDTH, CL_DEVICE_IMAGE2D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_WIDTH, CL_DEVICE_IMAGE3D_MAX_HEIGHT, CL_DEVICE_IMAGE3D_MAX_DEPTH
  clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &info->m_image2dMaxWidth, NULL);
  clGetDeviceInfo(device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t), &info->m_image2dMaxHeight, NULL);
  clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t), &info->m_image3dMaxWidth, NULL);
  clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t), &info->m_image3dMaxHeight, NULL);
  clGetDeviceInfo(device, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &info->m_image3dMaxDepth, NULL);
  // CL_DEVICE_EXTENSIONS: get device extensions, and if any then parse & log the string onto separate lines
  clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, MCL_MAX_STRING_LENGTH, &info->m_deviceExtensions, NULL); 
  // CL_DEVICE_PREFERRED_VECTOR_WIDTH_<type>
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(cl_uint), &info->m_vecWidthChar, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(cl_uint), &info->m_vecWidthShort, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(cl_uint), &info->m_vecWidthInt, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(cl_uint), &info->m_vecWidthLong, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(cl_uint), &info->m_vecWidthFloat, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(cl_uint), &info->m_vecWidthDouble, NULL);
  clGetDeviceInfo(device, CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(size_t), &info->m_profilingTimerResolution, NULL);
}

void mclPrintDeviceInfo(cl_device_id device) {
  mclDeviceInfo info;
  mclGetDeviceInfo(device,&info);
  printf("Device Info:\n");
  printf("  CL_DEVICE_NAME: \t\t\t%s\n", info.m_deviceName);
  printf("  CL_DEVICE_VENDOR: \t\t\t%s\n", info.m_deviceVendor);
  printf("  CL_DRIVER_VERSION: \t\t\t%s\n", info.m_driverVersion);
  if( info.m_deviceType & CL_DEVICE_TYPE_CPU ) {
    printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_CPU");
  }
  if( info.m_deviceType & CL_DEVICE_TYPE_GPU ) {
    printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_GPU");
  }
  if( info.m_deviceType & CL_DEVICE_TYPE_ACCELERATOR ) {
    printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_ACCELERATOR");
  }
  if( info.m_deviceType & CL_DEVICE_TYPE_DEFAULT ) {
    printf("  CL_DEVICE_TYPE:\t\t\t%s\n", "CL_DEVICE_TYPE_DEFAULT");
  }
  printf("  CL_DEVICE_MAX_COMPUTE_UNITS:\t\t%u\n", info.m_computeUnits);
  printf("  CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:\t%zu\n", info.m_workitemDims);
  printf("  CL_DEVICE_MAX_WORK_ITEM_SIZES:\t%zu / %zu / %zu \n", info.m_workItemSize[0], info.m_workItemSize[1], info.m_workItemSize[2]);
  printf("  CL_DEVICE_MAX_WORK_GROUP_SIZE:\t%zu\n", info.m_workgroupSize);
  printf("  CL_DEVICE_MAX_CLOCK_FREQUENCY:\t%u MHz\n", info.m_clockFrequency);
  printf("  CL_DEVICE_ADDRESS_BITS:\t\t%u\n", info.m_addressBits);
  printf("  CL_DEVICE_MAX_MEM_ALLOC_SIZE:\t\t%u MByte\n", (unsigned int)(info.m_maxMemAllocSize/ (1024 * 1024)));
  printf("  CL_DEVICE_GLOBAL_MEM_SIZE:\t\t%u MByte\n", (unsigned int)(info.m_globalMemSize/ (1024 * 1024)));
  printf("  CL_DEVICE_ERROR_CORRECTION_SUPPORT:\t%s\n", info.m_errorCorrectionSupport== CL_TRUE ? "yes" : "no");
  printf("  CL_DEVICE_LOCAL_MEM_TYPE:\t\t%s\n", info.m_localMemType == 1 ? "local" : "global");
  printf("  CL_DEVICE_LOCAL_MEM_SIZE:\t\t%u KByte\n", (unsigned int)(info.m_localMemSize / 1024));
  printf("  CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:\t%u KByte\n", (unsigned int)(info.m_constantBufferSize / 1024));
  if( info.m_queueProperties  & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ) {
    printf("  CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE");
  }
  if( info.m_queueProperties & CL_QUEUE_PROFILING_ENABLE ) {
    printf("  CL_DEVICE_QUEUE_PROPERTIES:\t\t%s\n", "CL_QUEUE_PROFILING_ENABLE");
  }
  printf("  CL_DEVICE_IMAGE_SUPPORT:\t\t%u\n", info.m_imageSupport);
  printf("  CL_DEVICE_MAX_READ_IMAGE_ARGS:\t%u\n", info.m_maxReadImageArgs);
  printf("  CL_DEVICE_MAX_WRITE_IMAGE_ARGS:\t%u\n", info.m_maxWriteImageArgs);
  printf("\n  CL_DEVICE_IMAGE <dim>");
  printf("\t\t\t2D_MAX_WIDTH\t %zu\n", info.m_image2dMaxWidth);
  printf("\t\t\t\t\t2D_MAX_HEIGHT\t %zu\n", info.m_image2dMaxHeight);
  printf("\t\t\t\t\t3D_MAX_WIDTH\t %zu\n", info.m_image3dMaxWidth);
  printf("\t\t\t\t\t3D_MAX_HEIGHT\t %zu\n", info.m_image3dMaxHeight);
  printf("\t\t\t\t\t3D_MAX_DEPTH\t %zu\n", info.m_image3dMaxDepth);
  if (info.m_deviceExtensions != 0)
    printf("\n  CL_DEVICE_EXTENSIONS:%s\n",info.m_deviceExtensions);
  else {
    printf("  CL_DEVICE_EXTENSIONS: None\n");
  }
  printf("  CL_DEVICE_PREFERRED_VECTOR_WIDTH_<t>\t");
  printf("CHAR %u, SHORT %u, INT %u,LONG %u, FLOAT %u, DOUBLE %u\n\n\n",
	 info.m_vecWidthChar, info.m_vecWidthShort, info.m_vecWidthInt, info.m_vecWidthLong,info.m_vecWidthFloat, info.m_vecWidthDouble);
  printf("  CL_DEVICE_PROFILING_TIMER_RESOLUTION:\t\t%ld nanosecond(s)\n", info.m_profilingTimerResolution);
}
