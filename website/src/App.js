import Dashboard from "./pages/Dashboard";
import { createGlobalStyle } from "styled-components";

const GlobalStyle = createGlobalStyle`
  * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Inter', sans-serif;
    color: #222222;
  }
`;

const App = () => {
	return (
		<>
			<GlobalStyle />
			<Dashboard />
		</>
	);
};

export default App;
