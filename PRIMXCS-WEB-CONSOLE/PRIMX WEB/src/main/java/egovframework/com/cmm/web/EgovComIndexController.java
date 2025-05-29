package egovframework.com.cmm.web;

import java.lang.reflect.Method;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;

import org.egovframe.rte.fdl.access.bean.AuthorityResourceMetadata;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;

import egovframework.com.cmm.IncludedCompInfoVO;
import egovframework.com.cmm.LoginVO;
import egovframework.com.cmm.annotation.IncludedInfo;
import egovframework.com.cmm.service.EgovProperties;
import egovframework.com.cmm.util.EgovHttpRequestHelper;
import egovframework.com.cmm.util.EgovUserDetailsHelper;
import egovframework.com.primx.fun.sys.service.PasswordSettingVO;
import egovframework.com.primx.fun.sys.service.PrimxSystemService;
import egovframework.com.primx.fun.usr.service.IpInfoDTO;
import egovframework.com.primx.fun.usr.service.PrimxUserService;
import egovframework.com.primx.fun.usr.service.UserRegistrationDTO;
import egovframework.com.uat.uia.service.EgovLoginService;

@Controller
public class EgovComIndexController {

	@Autowired
	private ApplicationContext applicationContext;

	private static final Logger LOGGER = LoggerFactory.getLogger(EgovComIndexController.class);

	/** EgovLoginService */
	@Resource(name = "loginService")
	private EgovLoginService loginService;

	@Resource(name = "primxUserService")
	private PrimxUserService primxUserService;

	@Resource(name = "primxSystemService")
	private PrimxSystemService primxSystemService;
	
	@Resource(name="authorityResource")
	private AuthorityResourceMetadata sessionResourceMetadata;

	@RequestMapping("/index.do")
	public String index(ModelMap model) {
		return "egovframework/com/cmm/EgovUnitMain";
	}

	@RequestMapping("/EgovContent.do")
	public String setContent(ModelMap model) throws Exception {

		sessionResourceMetadata.reload();

		// 비밀번호 설정일로부터 몇일이 지났는지 확인한다. ex) 3이면 비빌번호 설정후 3일 경과
		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();

		// 접속 가능 사용자인지 체크
		boolean isUserAvailable = primxUserService.checkUserUseYn(loginVO.getId());
		if(!isUserAvailable) {
			model.addAttribute("loginMessage", "사용자가 존재하지 않습니다.");
			return "egovframework/com/uat/uia/EgovLoginUsr";
		}

		// 사용자 만료일 체크
		int isUserExpiredDateCount = primxUserService.checkUserExpiredDate(loginVO.getId());
		if(isUserExpiredDateCount <= 0) {
			model.addAttribute("loginMessage", "계정이 만료 되었습니다. 관리자에게 문의 하시기 바랍니다.");
			return "egovframework/com/uat/uia/EgovLoginUsr";
		}
		else if(isUserExpiredDateCount <= 15) {
			model.addAttribute("alertMessage", "계정 만료기간이 " + isUserExpiredDateCount + "일 남았습니다. 관리자에게 계정 연장신청 하세요.");
		}

		Map<String, Object> passwordSettingInfo = primxSystemService.loadPasswordSettingInfo();
		PasswordSettingVO passwordSettingVO = (PasswordSettingVO) passwordSettingInfo.get("data");

		if(passwordSettingVO != null) {
			// 비밀번호 만료일 체크
			int isPasswordExpiredDateCount = primxUserService.checkUserPasswordExpiredDate(loginVO.getId());

			if(isPasswordExpiredDateCount <= 0) {
				model.addAttribute("alertMessage", "비밀번호를 변경해주시기 바랍니다.");
				model.addAttribute("action", "popUpProfile");
			}
			else if(passwordSettingVO.getPassNotifyBefore() >= isPasswordExpiredDateCount) {
				model.addAttribute("alertMessage", "패스워드 사용기간이 " + isPasswordExpiredDateCount + "일 남았습니다.");
				model.addAttribute("action", "popUpProfile");
			}
		}

		// 초기 비밀번호의 경우 비밀번호 변경 이후 메인 페이지로 갈 수 있도록 처리
		UserRegistrationDTO emplyrDetails = primxUserService.getUserDetails(loginVO.getId());
		
		String clientIp = CheckIp(EgovHttpRequestHelper.getCurrentRequest());
		System.out.println("clientIp : " + clientIp);
		boolean isIp = false;
		
		try {
			
			if(emplyrDetails.getIpList().size() > 0) {
				for(IpInfoDTO ip : emplyrDetails.getIpList()) {
					if(ip.getAllowedIp().equals(clientIp)) {
						isIp = true;
						break;
					}
				}
			}
			else {
				isIp = true;
			}
		} catch (Exception e) {
			// null 또는 없는 경우 접속 IP 지정이 안 된 상태
			isIp = true;
		}

		if(!isIp) {
			model.addAttribute("loginMessage", "접속 가능한 IP가 아닙니다.");
			return "egovframework/com/uat/uia/EgovLoginUsr";
		}

		// 사용자 권한 가져오기
		List<String> authList = EgovUserDetailsHelper.getAuthorities();
        String userAuth = authList.get(0);
		// gloval 설정에서 관리자인지 사용자인지에 따라 접근 권한을 비교하여 제한한다
		if(userAuth.contains(EgovProperties.getProperty("ACCESS.permission"))) {
			// 접근권한이 맞으면
			
			// 초기 비밀번호는 무조건 비밀번호 변경이 되어야 접근이 가능
			if(emplyrDetails.getPwdChanged().equals("N")) {
				return "egovframework/com/primx/fun/cmm/PrimxUnitPasswordChange";
			}

			return "egovframework/com/primx/fun/cmm/PrimxUnitMain";
		}
		else {
			// 접근권한이 없으면
			// 세션 종료시키기
		model.addAttribute("loginMessage", "접속 권한이 없습니다.");

			return "egovframework/com/uat/uia/EgovLoginUsr";
		}
	}

	private String CheckIp(HttpServletRequest req) {
		/*String sUserIp = req.getHeader("X-FORWARDED-FOR");
		if(sUserIp == null){
			sUserIp = req.getRemoteAddr();
		}
		*/
		String sUserIp = req.getHeader("X-Forwarded-For");
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("X-FORWARDED-FOR");
			System.out.println("sUserIp(X-FORWARDED-FOR) : " + sUserIp);
		}			
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("Proxy-Client-IP");
			System.out.println("sUserIp(Proxy-Client-IP) : " + sUserIp);
		}			
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("WL-Proxy-Client-IP");
			System.out.println("sUserIp(WL-Proxy-Client-IP) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("HTTP_CLIENT_IP");
			System.out.println("sUserIp(HTTP_CLIENT_IP) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("HTTP_X_FORWARDED_FOR");
			System.out.println("sUserIp(HTTP_X_FORWARDED_FOR) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("X-Forwarded-For");
			System.out.println("sUserIp(X-Forwarded-For) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("X-Real-IP");
			System.out.println("sUserIp(X-Real-IP) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("X-RealIP");
			System.out.println("sUserIp(X-RealIP) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getHeader("REMOTE_ADDR");
			System.out.println("sUserIp(REMOTE_ADDR) : " + sUserIp);
		}
		if ( sUserIp == null || sUserIp.length() == 0 || "unknown".equalsIgnoreCase(sUserIp)) {
			sUserIp = req.getRemoteAddr();
			System.out.println("sUserIp(getRemoteAddr) : " + sUserIp);
		}
		System.out.println("sUserIp : " + sUserIp);
		return sUserIp;
	}


	@RequestMapping("/EgovContent2.do")
	public String setContent2(ModelMap model) throws Exception {

		// 설정된 비밀번호 유효기간을 가져온다. ex) 180이면 비밀번호 변경후 만료일이 앞으로 180일
		String propertyExpirePwdDay = EgovProperties.getProperty("Globals.ExpirePwdDay");
		int expirePwdDay = 0 ;
		try {
			expirePwdDay =  Integer.parseInt(propertyExpirePwdDay);
		} catch (NumberFormatException Nfe) {
			LOGGER.debug("convert expirePwdDay Err : "+Nfe.getMessage());
		}

		model.addAttribute("expirePwdDay", expirePwdDay);

		// 비밀번호 설정일로부터 몇일이 지났는지 확인한다. ex) 3이면 비빌번호 설정후 3일 경과
		LoginVO loginVO = (LoginVO) EgovUserDetailsHelper.getAuthenticatedUser();
		model.addAttribute("loginVO", loginVO);
		int passedDayChangePWD = 0;
		if ( loginVO != null ) {
			LOGGER.debug("===>>> loginVO.getId() = "+loginVO.getId());
			LOGGER.debug("===>>> loginVO.getUniqId() = "+loginVO.getUniqId());
			LOGGER.debug("===>>> loginVO.getUserSe() = "+loginVO.getUserSe());
			// 비밀번호 변경후 경과한 일수
			passedDayChangePWD = loginService.selectPassedDayChangePWD(loginVO);
			LOGGER.debug("===>>> passedDayChangePWD = "+passedDayChangePWD);
			model.addAttribute("passedDay", passedDayChangePWD);
		}

		// 만료일자로부터 경과한 일수 => ex)1이면 만료일에서 1일 경과
		model.addAttribute("elapsedTimeExpiration", passedDayChangePWD - expirePwdDay);

//		return "egovframework/com/primx/fun/cmm/PrimxUnitMain";
		return "egovframework/com/cmm/EgovUnitMain";
	}
	
	@RequestMapping("/EgovLeft2.do")
	public String setLeftMenu2(ModelMap model) {

		Map<Integer, IncludedCompInfoVO> map = new TreeMap<Integer, IncludedCompInfoVO>();
		RequestMapping rmAnnotation;
		IncludedInfo annotation;
		IncludedCompInfoVO zooVO;

		/*
		 * EgovLoginController가 AOP Proxy되는 바람에 클래스를 reflection으로 가져올 수 없음
		 */
		try {
			Class<?> loginController = Class.forName("egovframework.com.uat.uia.web.EgovLoginController");
			Method[] methods = loginController.getMethods();
			for (int i = 0; i < methods.length; i++) {
				annotation = methods[i].getAnnotation(IncludedInfo.class);

				if (annotation != null) {
					LOGGER.debug("Found @IncludedInfo Method : {}", methods[i]);
					zooVO = new IncludedCompInfoVO();
					zooVO.setName(annotation.name());
					zooVO.setOrder(annotation.order());
					zooVO.setGid(annotation.gid());

					rmAnnotation = methods[i].getAnnotation(RequestMapping.class);
					if ("".equals(annotation.listUrl()) && rmAnnotation != null) {
						zooVO.setListUrl(rmAnnotation.value()[0]);
					} else {
						zooVO.setListUrl(annotation.listUrl());
					}
					map.put(zooVO.getOrder(), zooVO);
				}
			}
		} catch (ClassNotFoundException e) {
			LOGGER.error("No egovframework.com.uat.uia.web.EgovLoginController!!");
		}
		/* 여기까지 AOP Proxy로 인한 코드 */

		/*@Controller Annotation 처리된 클래스를 모두 찾는다.*/
		Map<String, Object> myZoos = applicationContext.getBeansWithAnnotation(Controller.class);
		LOGGER.debug("How many Controllers : ", myZoos.size());
		for (final Object myZoo : myZoos.values()) {
			Class<? extends Object> zooClass = myZoo.getClass();

			Method[] methods = zooClass.getMethods();
			LOGGER.debug("Controller Detected {}", zooClass);
			for (int i = 0; i < methods.length; i++) {
				annotation = methods[i].getAnnotation(IncludedInfo.class);

				if (annotation != null) {
					//LOG.debug("Found @IncludedInfo Method : " + methods[i] );
					zooVO = new IncludedCompInfoVO();
					zooVO.setName(annotation.name());
					zooVO.setOrder(annotation.order());
					zooVO.setGid(annotation.gid());
					/*
					 * 목록형 조회를 위한 url 매핑은 @IncludedInfo나 @RequestMapping에서 가져온다
					 */
					rmAnnotation = methods[i].getAnnotation(RequestMapping.class);
					if ("".equals(annotation.listUrl())) {
						zooVO.setListUrl(rmAnnotation.value()[0]);
					} else {
						zooVO.setListUrl(annotation.listUrl());
					}

					map.put(zooVO.getOrder(), zooVO);
				}
			}
		}

		model.addAttribute("resultList", map.values());

		LOGGER.debug("EgovComIndexController index is called ");

		return "egovframework/com/cmm/EgovUnitLeft";
	}

	// context-security.xml 설정
	// csrf="true"인 경우 csrf Token이 없는경우 이동하는 페이지
	// csrfAccessDeniedUrl="/egovCSRFAccessDenied.do"
	@RequestMapping("/egovCSRFAccessDenied.do")
	public String egovCSRFAccessDenied() {
		return "egovframework/com/cmm/error/csrfAccessDenied";
	}


	/**
	 * adml, admx xml 파싱후 tree 구조 생성 샘플
	 * @param model
	 * @return
	 * @throws Exception
	 */
	@RequestMapping("/xmlparseSample.do")
	public String xmlparseSample(ModelMap model) throws Exception {
		LOGGER.debug("xmlParseSample page");
		return "egovframework/com/xmlparseSample";
	}
}
