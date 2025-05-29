package egovframework.com.primx.fun.cmm.util;

import egovframework.com.cmm.service.EgovProperties;

public class FileSizeCheck {
    
    /*
     * # 파일 사이즈 단위 B, KB, MB, GB
     * PRIMX.file.sizeUnit = MB
     * # 파일 사이즈
     * PRIMX.file.size = 1024
     * 
     * 단 eGov 파일 사이즈 제한이 있음으로 
     * custom multi file resolver 확인할 것 (context-common.xml)
     */
    public static long getMaxFileSize() {
        String sizeUnit = EgovProperties.getProperty("PRIMX.file.sizeUnit");
        long size = Long.parseLong(EgovProperties.getProperty("PRIMX.file.size"));

        if(sizeUnit.equals("B")) {
            return size;
        }
        else if(sizeUnit.equals("KB")) {
            return size * 1024;
        }
        else if(sizeUnit.equals("MB")) {
            return size * 1024 * 1024;
        }
        else if(sizeUnit.equals("GB")) {
            return size * 1024 * 1024 * 1024;
        }
        else {
            return size;
        }
    }
}
