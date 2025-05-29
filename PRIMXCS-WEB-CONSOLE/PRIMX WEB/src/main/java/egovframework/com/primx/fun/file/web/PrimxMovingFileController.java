package egovframework.com.primx.fun.file.web;

import java.util.Map;

import javax.annotation.Resource;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import egovframework.com.primx.fun.file.service.MoveFileSearchDTO;
import egovframework.com.primx.fun.file.service.PrimxMovingFileService;

/**
 * 파일 이동 관련 기능
 * @author 김영우
 * @since 2024.12.3
 * @version 1.0
 * @see
 *
 * <pre>
 * << 개정이력(Modification Information) >>
 *
 *   수정일      수정자          수정내용
 *  -------    --------    ---------------------------
 *  2024.12.03  김영우          최초 생성
 *
 *  </pre>
 */

@Controller
@RequestMapping(value = "/primx/fun/file")
public class PrimxMovingFileController {

	@Resource(name = "primxMovingFileService")
	private PrimxMovingFileService primxMovingFileService;

	@RequestMapping(value = "/view.do")
	public String getView(ModelMap model) throws Exception {
		
		return "egovframework/com/primx/fun/file/moveFileHistory";
	}

	@ResponseBody
	@RequestMapping(value = "/loadMoveFileHistory.do", method = RequestMethod.POST)
	public Map<String, Object> loadMoveFileHistory(MoveFileSearchDTO searchDTO) throws Exception {
		searchDTO.convertUserTimeToUTC();

		System.out.println("searchDTO : " + searchDTO.toString());
		return primxMovingFileService.loadMoveFileHistory(searchDTO);
	}

	@ResponseBody
	@RequestMapping(value = "/loadOriginalFileInfo.do", method = RequestMethod.POST)
	public Map<String, Object> loadOriginalFileInfo(String fileKey) throws Exception {
		return primxMovingFileService.loadOriginalFileInfo(fileKey);
	}

	@ResponseBody
	@RequestMapping(value = "/loadMoveFileUsageHistory.do", method = RequestMethod.POST)
	public Map<String, Object> loadMoveFileUsageHistory(String fileKey) throws Exception {
		return primxMovingFileService.loadMoveFileUsageHistory(fileKey);
	}
}
