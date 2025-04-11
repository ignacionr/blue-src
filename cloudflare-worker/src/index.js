export default {
    async fetch(request, env, ctx) {
      const url = new URL(request.url);
      const path = url.pathname;
  
      // Skip if path is root or has an extension
      const isRoot = path === '/';
      const hasExtension = /\.[^\/]+$/.test(path);
  
      if (isRoot || hasExtension) {
        return fetch(request);
      }
  
      const response = await fetch(request);
  
      // Clone and modify headers
      const newHeaders = new Headers(response.headers);
      newHeaders.set("Content-Type", "application/json");
      newHeaders.set("cache-control", "no-store");
      newHeaders.delete("Content-Disposition");
  
      return new Response(response.body, {
        status: response.status,
        statusText: response.statusText,
        headers: newHeaders,
      });
    }
  };
  