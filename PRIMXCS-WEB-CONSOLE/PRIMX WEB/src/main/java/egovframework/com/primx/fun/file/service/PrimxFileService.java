package egovframework.com.primx.fun.file.service;

import org.springframework.web.multipart.MultipartHttpServletRequest;

import java.util.Map;

public interface PrimxFileService {
    Map<String,FileVO> uploadFiles(boolean isGroup, String uploadDir, String extWhiteList, MultipartHttpServletRequest mRequest) throws Exception;
}
