import styled from "styled-components";

const Container = styled.div`
	height: 45px;
	display: flex;
	justify-content: center;
	align-items: center;
	background: #5b5b5b;
	box-shadow: 0px 1px 1px rgba(0, 0, 0, 0.25);
	margin-top: 30px;
`;

const Text = styled.span`
	font-style: normal;
	font-weight: 600;
	font-size: 20px;
	line-height: 24px;
	color: #fdfdfd;
`;

export { Container, Text };
