package egovframework.com.primx.fun.file.service;
import java.util.Map;

public interface PrimxMovingFileService {
    Map<String,Object> loadMoveFileHistory(MoveFileSearchDTO searchDTO) throws Exception;
    Map<String,Object> loadOriginalFileInfo(String fileKey) throws Exception;
    Map<String,Object> loadMoveFileUsageHistory(String fileKey) throws Exception;
}
